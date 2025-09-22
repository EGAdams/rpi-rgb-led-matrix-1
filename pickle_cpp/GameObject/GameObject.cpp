#include "GameObject.h"
#include "../Arduino/Arduino.h"

#if !defined(ARDUINO_TEENSY41)
#include <string>
#endif

// No need to forward-declare History or include <memory> here // another git test
// because we treat _history as a raw pointer per the header.

// -----------------------------
// Ctors
// -----------------------------
GameObject::GameObject(
    Team* team_a,
    Team* team_b,
    PinState* pinState,
    PinInterface* pinInterface,
    GameState* gameState,
    GameTimer* gameTimer,
    Inputs* gameInputs,
    GameModes* gameModes,
    ScoreBoard* scoreBoard
) {
    _team_a       = team_a;
    _team_b       = team_b;
    _pinState     = pinState;
    _pinInterface = pinInterface;
    _gameState    = gameState;
    _gameTimer    = gameTimer;
    _gameInputs   = gameInputs;
    _gameModes    = gameModes;
    _scoreBoard   = scoreBoard;

    // Own History with shared_ptr for safe lifetime management
    _history      = std::make_shared<History>();
    if (_history) {
        std::cout << "[GameObject] Created History at " << _history.get() << std::endl;
    }
}

GameObject::GameObject() {
    ClockTimer* clockTimer = new ClockTimer();
    _gameTimer   = new GameTimer();
    _team_a      = new Team(TEAM_A);
    _team_b      = new Team(TEAM_B);
    _team_a->setOpposingTeam(_team_b);
    _team_b->setOpposingTeam(_team_a);
    _team_a->setServe(1);
    _pinState    = new PinState();
    _pinInterface= new PinInterface(_pinState);
    _gameState   = new GameState();
    _rules       = new Rules(_gameState->getGameMode()); // SINGLES / DOUBLES

    ColorManager* colorManager = new ColorManager();
    FontManager*  fontManager  = new FontManager();
    IDisplay* display          = new ConsoleDisplay(colorManager);

    _scoreBoard  = new ScoreBoard(_pinInterface, _team_a, _team_b, _rules,
                                  clockTimer, _gameState, display, fontManager, colorManager);
    _gameInputs  = new Inputs(_team_a, _team_b, _pinInterface, _scoreBoard);
    _gameModes   = new GameModes(_team_a, _team_b, _pinInterface, _rules, _scoreBoard);
    _reset       = new Reset(_pinInterface);

    // Own History with shared_ptr for safe lifetime management
    _history     = std::make_shared<History>();
    if (_history) {
        std::cout << "[GameObject] Created History at " << _history.get() << std::endl;
    }

#if !defined(ARDUINO_TEENSY41)
    _logger = new Logger("GameObject");
    _logger->logUpdate("GameObject constructed.", __FUNCTION__);
#endif

    _blinkController = new BlinkController(_pinInterface, _team_a, _team_b);
    _team_a->attach(_blinkController);
    _team_b->attach(_blinkController);
}

GameObject::~GameObject() {}

// -----------------------------
// Loop / state
// -----------------------------
void GameObject::loopGame() {
    // int rotary_position = _gameInputs->readRotary(_gameState, _reset);
    GameTimer::gameDelay(MAIN_GAME_DELAY);
    // _gameModes->setGameMode(rotary_position, _gameState, _history); // 080625
}

bool GameObject::gameRunning() {
    std::cout << "gameRunning() called, returning true... " << std::endl;
    return true;
}

void GameObject::start(){}

// -----------------------------
// Getters
// -----------------------------
PinInterface*   GameObject::getPinInterface()   { return _pinInterface; }
GameState*      GameObject::getGameState()      { return _gameState; }
Team*           GameObject::getTeamA()          { return _team_a; }
Team*           GameObject::getTeamB()          { return _team_b; }
PinState*       GameObject::getPinState()       { return _pinState; }
GameTimer*      GameObject::getGameTimer()      { return _gameTimer; }
Inputs*         GameObject::getGameInputs()     { return _gameInputs; }
GameModes*      GameObject::getGameModes()      { return _gameModes; }
ScoreBoard*     GameObject::getScoreBoard()     { return _scoreBoard; }
Rules*          GameObject::getRules()          { return _rules; }
Reset*          GameObject::getReset()          { return _reset; }
std::shared_ptr<History> GameObject::history() const { 
    std::cout << "[GameObject] history() called, returning shared_ptr to: " << _history.get() 
              << " (use_count: " << _history.use_count() << ")" << std::endl;
    return _history; 
}
Logger*         GameObject::getLogger()         { return _logger; }
BlinkController*GameObject::getBlinkController(){ return _blinkController; }

// -----------------------------
// Setters
// -----------------------------
void GameObject::setTeamA(Team* team)                    { _team_a = team; }
void GameObject::setTeamB(Team* team)                    { _team_b = team; }
void GameObject::setPinState(PinState* pinState)         { _pinState = pinState; }
void GameObject::setPinInterface(PinInterface* pi)       { _pinInterface = pi; }
void GameObject::setGameState(GameState* gs)             { _gameState = gs; }
void GameObject::setGameTimer(GameTimer* gt)             { _gameTimer = gt; }
void GameObject::setGameInputs(Inputs* inputs)           { _gameInputs = inputs; }
void GameObject::setGameModes(GameModes* modes)          { _gameModes = modes; }
void GameObject::setScoreBoard(ScoreBoard* sb)           { _scoreBoard = sb; }
void GameObject::setRules(Rules* rules)                  { _rules = rules; }
void GameObject::setReset(Reset* reset)                  { _reset = reset; }
void GameObject::setHistory(std::shared_ptr<History> history) { 
    std::cout << "[GameObject] setHistory called, old: " << _history.get() 
              << " (use_count: " << _history.use_count() << "), new: " << history.get() 
              << " (use_count: " << history.use_count() << ")" << std::endl;
    _history = history; 
}
void GameObject::setLogger(Logger* logger)               { _logger = logger; }
void GameObject::setBlinkController(BlinkController* b)  { _blinkController = b; }

// -----------------------------
// Actions
// -----------------------------
void GameObject::playerScore(int player) {
    std::cout << "GameObject::playerScore(" << player << ") called..." << std::endl;
}

void GameObject::undo() {
    std::cout << "GameObject::undo() called..." << std::endl;
    GameStateManager* gameStateManager = new GameStateManager(_scoreBoard);
    auto hist = _history; // Pin lifetime with local shared_ptr
    gameStateManager->setPreviousGameState(_gameState, _team_a, _team_b, *hist);
    delete gameStateManager;

    _gameState->setPlayerButton(0);
    _scoreBoard->update();
}

void GameObject::resetMatch() {
    std::cout << "GameObject::resetMatch() called..." << std::endl;
    _gameState->setStarted(0);
    auto hist = _history; // Pin lifetime with local shared_ptr
    _gameModes->gameStart(_gameState, *hist); // Pass reference
}
