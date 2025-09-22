# History Use‑After‑Free Fix & Debug Playbook (C++)  
_Date: 2025-08-23 19:08:37_

# History Use‑After‑Free Fix & Debug Playbook (C++)  

> **Symptom from your logs**
>
> ```text
> *** PROTECTED_HISTORY: UAF guard tripped in size, this=0x5555556234d0
> ```
>
> Your `History` object was destroyed, yet later accessed (e.g., `size()` as part of `HISTORY_POP`). This is a classic **use‑after‑free** caused by **dangling `History*`**.

This playbook gives **exact file changes** and **commands** for your filesystem‑assistant to implement and verify. It standardizes lifetime by switching to **`std::shared_ptr<History>` end‑to‑end** and passing **`History&`** into functions (while the caller keeps a local `shared_ptr` to pin the lifetime).

---

## Objectives

1. Eliminate dangling `History*` by using `std::shared_ptr<History>` everywhere a pointer was previously used.  
2. Update APIs to accept `History&` or `std::shared_ptr<History>` and adjust call sites accordingly.  
3. Keep the caller holding a `shared_ptr` **locally** during calls to guarantee the object outlives the call.  
4. Rebuild with **ASan/UBSan** and verify via **GDB** breakpoints.

---

## Pre‑flight

- Work in the project root (replace as needed):  
  ```bash
  cd ~/rpi-rgb-led-matrix/pickle_cpp
  ```

- Create a **safety branch** and backup patch:
  ```bash
  git checkout -b fix/history-uaf
  git status
  ```

- Quick inventory (shows likely raw‑pointer usage):
  ```bash
  rg -n "\bHistory\s*\*"
  rg -n "getHistory\s*\("
  rg -n "HISTORY_POP\s*\("
  ```

---

## Step 1 — Make `GameObject` own `History` via `shared_ptr`

> Replace any raw `History*` member with `std::shared_ptr<History>` and provide a safe accessor.

### 1A) **GameObject.h** (or equivalent class that “owns” history)

**Before** (pattern):
```cpp
class GameObject { 
  // ...
  History* _history;  // raw pointer (danger)
  // ...
public:
  History* getHistory(); // returns raw pointer
  // ...
};
```

**After**:
```cpp
#include <memory>

class GameObject {
  // ...
  std::shared_ptr<History> _history;  // owns lifetime
  // ...
public:
  // Prefer returning shared_ptr so callers can extend lifetime locally
  std::shared_ptr<History> history() const { return _history; }
  // If you still have getHistory(), deprecate it or make it return _history.get() only for legacy code
};
```

### 1B) **GameObject.cpp** (constructors)

**Before** (pattern that also triggered your earlier compile error):
```cpp
_history = std::make_shared<History>(); // but member is History*
```

**After**:
```cpp
_history = std::make_shared<History>();
```

If there are multiple constructors, apply the same initialization in each.

### 1C) Remove/limit any direct `delete _history;` (should not exist with `shared_ptr`).

---

## Step 2 — Update APIs to avoid raw pointers

Prefer **reference** parameters for operations, while the **caller** holds a `shared_ptr` locally to pin lifetime.

### 2A) **GameStateManager.h**

**Before**:
```cpp
void setPreviousGameState(GameState* gameState,
                          Team* team_a,
                          Team* team_b,
                          History* history);
```

**After** (recommended):
```cpp
void setPreviousGameState(GameState* gameState,
                          Team* team_a,
                          Team* team_b,
                          History& history);
```

> Rationale: taking a reference avoids additional ownership costs and makes it impossible to pass `nullptr`. The **caller must** keep a `std::shared_ptr<History>` alive in its scope during the call (see Step 3).

*(Alternative if you prefer explicit ownership in the signature:)*
```cpp
void setPreviousGameState(GameState* gameState,
                          Team* team_a,
                          Team* team_b,
                          std::shared_ptr<History> history);
```

### 2B) **GameStateManager.cpp**

Change usages from `history->...` to `history....` when you switch to `History&`:

```diff
- GameState savedState = HISTORY_POP(*history);
+ GameState savedState = HISTORY_POP(history);
```

and any other `history->` occurrences:

```diff
- if (history->empty()) { ... }
+ if (history.empty()) { ... }
```

---

## Step 3 — Fix call sites (State handlers, etc.)

Wherever you previously did:
```cpp
auto* history = gameObject->getHistory();   // raw pointer
manager.setPreviousGameState(gs, a, b, history);
```

Switch to:
```cpp
auto hist = gameObject->history();          // shared_ptr<History>
// Keep `hist` alive in this scope to pin lifetime:
manager.setPreviousGameState(gs, a, b, *hist);   // pass by reference
```

> **Important:** That local `hist` variable is what guarantees `History` stays alive for the duration of the call chain.

---

## Step 4 — Adjust helper macros/utilities

If you have a `HISTORY_POP` macro, make sure it expects a **reference**, not a pointer.

**Before** (pointer-ish):
```cpp
#define HISTORY_POP(H) (H)->pop()
```

**After** (reference):
```cpp
#define HISTORY_POP(H) (H).pop()
```

And update all sites to pass a `History&` (as shown in Step 3).

---

## Step 5 — Add a creation log (optional but helpful)

In `GameObject.cpp` (after constructing history):
```cpp
if (_history) {
  std::cout << "[GameObject] Created History at " << _history.get() << std::endl;
}
```

This lets you correlate addresses with the UAF address in logs.

---

## Step 6 — Build with sanitizers

Use address + undefined sanitizers (and frame pointers) for a debug build:

```bash
g++ -std=c++17 -pthread -O0 -g -fsanitize=address,undefined -fno-omit-frame-pointer \
  -o run_pickle_listener @ofiles.rsp \
  -L/home/$USER/rpi-rgb-led-matrix/lib \
  -Wl,--start-group -lrgbmatrix -Wl,--end-group -lrt -lm -lpthread
```

> If you’re using a Makefile, add these flags to `CXXFLAGS`/`LDFLAGS` in your debug config.

Run it. If any UAF remains, ASan will print a precise **allocation** and **free** backtrace.

---

## Step 7 — Prove the fix (and backstop with GDB)

Set breakpoints to ensure the same `History` address is not destroyed before use:

```gdb
(gdb) break 'History::~History()'
(gdb) break 'History::_uafGuard(char const*)'
(gdb) run
# When ~History() triggers, do:
(gdb) bt
# When _uafGuard triggers, do:
(gdb) bt
```

> With the refactor, `_uafGuard` should **no longer trigger** for legitimate calls. If it does, the backtrace will show the accidental destroyer.

---

## Step 8 — Mechanical edits checklist (search/replace aids)

Use ripgrep to find all relevant sites:

```bash
# 1) Raw History* members
rg -n "\bHistory\s*\*"

# 2) Legacy getters
rg -n "getHistory\s*\("

# 3) Macro uses
rg -n "HISTORY_POP\s*\("

# 4) Any direct deletes (should not exist with shared_ptr)
rg -n "delete\s+\*?\s*history"
```

For each file with a `History*` member:
- Replace the member with `std::shared_ptr<History>` and update constructor(s).
- Add `#include <memory>`.

For each function taking `History*`:
- Change to `History&` (or `std::shared_ptr<History>` per team preference).
- Update call sites to hold a local `shared_ptr` and pass `*hist` (or `hist`).

For each `HISTORY_POP(history)` call:
- Ensure the macro expects a reference and pass a `History&` variable (see Step 4).

---

## Step 9 — Optional defensive improvements

- **Disallow copy/move** of `History` (to prevent accidental shallow copies of mutexes, etc.):  
  In `History.h`:
  ```cpp
  History(const History&) = delete;
  History& operator=(const History&) = delete;
  History(History&&) = delete;
  History& operator=(History&&) = delete;
  ```

- If any background threads stash a raw `History*`, update them to store `std::weak_ptr<History>` and lock to `shared_ptr` when needed:
  ```cpp
  std::weak_ptr<History> _historyWeak;

  if (auto h = _historyWeak.lock()) {
      // use *h safely
  }
  ```

---

## Quick sanity test (manual)

Add a trivial push/pop sequence in a safe spot (e.g., a one‑off test harness or right after history creation) to ensure `push`/`pop` work and integrity checks pass:

```cpp
auto hist = gameObject->history();
GameState gs;
hist->push(gs);
auto out = hist->pop();
std::cout << "[Sanity] History size now: " << hist->size() << std::endl;
```

Expected: size returns `0`, no UAF guard trips, no integrity failures.

---

## Commit & rollback

```bash
git add -A
git commit -m "Fix(History): replace raw History* with shared_ptr, pass History& to APIs; update HISTORY_POP; ASan/UBSan build"
# To rollback:
# git reset --hard HEAD~1
```

---

## Why this solves your crash

- Calls like `setPreviousGameState(...)` now receive a **`History&`** that’s guaranteed non‑null.  
- The **caller** holds a `shared_ptr` (`auto hist = gameObject->history();`) so the `History` lives at least as long as the call.  
- No more mismatched ownership (the source of your earlier `shared_ptr`→`History*` compile error).  
- ASan + `_uafGuard` remain as backstops.

---

## Notes from your log context

- The crash happened immediately after printing the retrieved pointer address (`0x5555556234d0`) and before doing real work, showing the **lifetime** was already over at the first method call.  
- With this playbook, that exact pattern won’t reproduce because the lifetime is pinned by the local `shared_ptr` variable.

---

### Done.
When all steps are complete, rebuild and re-run your original scenario to confirm the UAF guard no longer trips.
