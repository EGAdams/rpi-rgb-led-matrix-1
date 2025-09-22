#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <cassert>
#include <sstream>

// Mock classes for testing
class IPairingBlinker {
public:
    virtual ~IPairingBlinker() = default;
};

class IInputs {
public:
    virtual ~IInputs() = default;
    virtual int read_mcp23017_value() = 0;
};

class IInputWithTimer {
public:
    virtual ~IInputWithTimer() = default;
    virtual int  getInput() = 0;
    virtual long getTimeSlept() const = 0;
};

// Concrete InputWithTimer class (implementation provided)
class InputWithTimer : public IInputWithTimer {
public:
    InputWithTimer( std::shared_ptr<IPairingBlinker> blinker,
                    std::shared_ptr<IInputs> inputs )
        : m_blinker( blinker ), m_inputs( inputs ), m_timeSlept( 0 ) {}

    int getInput() override {
        using namespace std::chrono;
        int REMOTE_INPUT = 1;

        auto start = steady_clock::now();
        int value = 0;
        if ( REMOTE_INPUT == 1 ) {
            value = m_inputs->read_mcp23017_value();
        } else {
            std::cout << "Enter a value: ";
            std::cin >> value;
        }
        auto end = steady_clock::now();

        m_timeSlept = duration_cast<milliseconds>( end - start ).count();
        return value;
    }

    long getTimeSlept() const override {
        return m_timeSlept;
    }

private:
    std::shared_ptr<IPairingBlinker> m_blinker;
    std::shared_ptr<IInputs>         m_inputs;
    long                             m_timeSlept;
};

// Mock implementations
class MockPairingBlinker : public IPairingBlinker {
    // No behavior needed for this mock
};

class MockInputs : public IInputs {
public:
    explicit MockInputs(int value) : m_value(value), m_called(false) {}

    int read_mcp23017_value() override {
        m_called = true;
        return m_value;
    }

    bool wasCalled() const {
        return m_called;
    }

private:
    int m_value;
    bool m_called;
};

// Test harness
void testInputWithTimer() {
    // Mock objects
    auto blinker = std::make_shared<MockPairingBlinker>();
    auto inputs = std::make_shared<MockInputs>(42);
    int REMOTE_INPUT = 1;

    // Create InputWithTimer
    InputWithTimer inputWithTimer(blinker, inputs);

    // Test 1: Mock input value
    REMOTE_INPUT = 1;
    int value = inputWithTimer.getInput();
    assert(value == 42 && "Test 1 failed: getInput() did not return mock value");
    assert(inputs->wasCalled() && "Test 1 failed: read_mcp23017_value() was not called");
    assert(inputWithTimer.getTimeSlept() >= 0 && "Test 1 failed: Time slept should be non-negative");

    // Test 2: Manual input
    REMOTE_INPUT = 0;
    std::istringstream fakeInput("123\n");
    std::cin.rdbuf(fakeInput.rdbuf()); // Redirect std::cin
    value = inputWithTimer.getInput();
    assert(value == 123 && "Test 2 failed: getInput() did not return manual input value");
    assert(inputWithTimer.getTimeSlept() >= 0 && "Test 2 failed: Time slept should be non-negative");

    // Test 3: No input
    REMOTE_INPUT = 1;
    inputs = std::make_shared<MockInputs>(0);
    InputWithTimer noInputTimer(blinker, inputs);
    value = noInputTimer.getInput();
    assert(value == 0 && "Test 3 failed: getInput() did not return zero for no input");
    assert(inputs->wasCalled() && "Test 3 failed: read_mcp23017_value() was not called");

    // Test 4: Edge case - large input value
    inputs = std::make_shared<MockInputs>(999999);
    InputWithTimer largeValueTimer(blinker, inputs);
    value = largeValueTimer.getInput();
    assert(value == 999999 && "Test 4 failed: getInput() did not handle large input value");

    // Test 5: Edge case - negative input value
    inputs = std::make_shared<MockInputs>(-42);
    InputWithTimer negativeValueTimer(blinker, inputs);
    value = negativeValueTimer.getInput();
    assert(value == -42 && "Test 5 failed: getInput() did not handle negative input value");

    std::cout << "All tests passed!" << std::endl;
}

int main() {
    testInputWithTimer();
    return 0;
}
