#include <iostream>
struct DebugInitializer {
    DebugInitializer() {
        std::cout << "DEBUG: Global/static initialization started" << std::endl;
    }
};

DebugInitializer debugInit;
#include <gtest/gtest.h>
#include "../PairingModeState/PairingModeState.h"
#include "../RemoteListenerContext/RemoteListenerContext.h"
#include "../ScoreBoard/ScoreBoard.h"
#include "../GameObject/GameObject.h"
#include "../GameState/GameState.h"
#include "../Reset/Reset.h"
#include "../RemoteLocker/RemoteLocker.h"
#include "../Inputs/Inputs.h"
#include "../RemoteInputWithTimer/RemoteInputWithTimer.h"
#include "../BlankBlinker/BlankBlinker.h"
#include "../ScoreboardBlinker/ScoreboardBlinker.h"
#include "../RemoteGameInput/RemoteGameInput.h"
#include "../RemotePairingScreen/RemotePairingScreen.h"
#include "../StateMachine/StateMachine.h"

class PairingModeStateTest : public ::testing::Test {
    protected:
        GameState* gameState;
        GameObject* gameObject;
        Reset* reset;
        RemoteLocker* remoteLocker;
        Inputs* inputs;
        ScoreBoard* scoreboard;
        std::shared_ptr<PairingBlinker> pairingBlinker;
        std::shared_ptr<BlankBlinker> blankBlinker;
        std::shared_ptr<ScoreboardBlinker> sleepingBlinker;
        std::unique_ptr<IInputWithTimer> pairingInputWithTimer;
        std::unique_ptr<IInputWithTimer> noBlinkInputWithTimer;
        std::unique_ptr<IInputWithTimer> sleepingInputWithTimer;
        std::unique_ptr<IGameInput> gameInput;
        std::unique_ptr<RemotePairingScreen> remotePairingScreen;
        std::unique_ptr<RemoteListenerContext> context;
        std::unique_ptr<PairingModeState> pairingModeState;
        
    void SetUp() override {
        std::cout << "DEBUG: Entering SetUp()" << std::endl;

        // Initialize game state and game object
        gameState = new GameState();
        std::cout << "DEBUG: gameState initialized at " << gameState << std::endl;
        
        gameObject = new GameObject(gameState, new ConsoleDisplay(new ColorManager()));
        std::cout << "DEBUG: gameObject initialized at " << gameObject << std::endl;

        // Obtain the scoreboard and set its font
        scoreboard = gameObject->getScoreBoard();
        std::cout << "DEBUG: scoreboard initialized at " << scoreboard << std::endl;
        scoreboard->setLittleDrawerFont("fonts/8x13B.bdf");

        // Construct the game object properly (assigning to the class member)
        ColorManager* colorManager = new ColorManager();
        IDisplay* display = static_cast<IDisplay*>(new ConsoleDisplay(colorManager));
        this->gameObject = new GameObject(gameState, display);

        // Initialize reset and remoteLocker correctly
        this->reset = new Reset(
            gameObject->getPlayer1(), 
            gameObject->getPlayer2(), 
            gameObject->getPinInterface(), 
            gameState
        );

        this->remoteLocker = new RemoteLocker(gameState);

        // Create blinkers and remote pairing screen
        pairingBlinker = std::make_shared<PairingBlinker>(scoreboard);
        blankBlinker = std::make_shared<BlankBlinker>();
        sleepingBlinker = std::make_shared<ScoreboardBlinker>(scoreboard);
        remotePairingScreen = std::make_unique<RemotePairingScreen>(scoreboard);

        std::cout << "DEBUG: pairingBlinker created at " << pairingBlinker.get() << std::endl;
        std::cout << "DEBUG: blankBlinker created at " << blankBlinker.get() << std::endl;
        std::cout << "DEBUG: sleepingBlinker created at " << sleepingBlinker.get() << std::endl;

        // Create inputs object
        inputs = new Inputs(gameObject->getPlayer1(), gameObject->getPlayer2(), gameObject->getPinInterface(), gameState);
        std::cout << "DEBUG: inputs created at " << inputs << std::endl;

        // Create RemoteInputWithTimer objects
        std::shared_ptr<Blinker> castedPairingBlinker = pairingBlinker;
        std::cout << "DEBUG: castedPairingBlinker = " << castedPairingBlinker.get() << std::endl;

        pairingInputWithTimer = std::make_unique<RemoteInputWithTimer>(castedPairingBlinker, inputs, 4000);
        noBlinkInputWithTimer = std::make_unique<RemoteInputWithTimer>(
            std::static_pointer_cast<Blinker>(blankBlinker), inputs, 4000);
        sleepingInputWithTimer = std::make_unique<RemoteInputWithTimer>(
            std::static_pointer_cast<Blinker>(sleepingBlinker), inputs, 4000);

        std::cout << "DEBUG: pairingInputWithTimer created at " << pairingInputWithTimer.get() << std::endl;
        std::cout << "DEBUG: noBlinkInputWithTimer created at " << noBlinkInputWithTimer.get() << std::endl;
        std::cout << "DEBUG: sleepingInputWithTimer created at " << sleepingInputWithTimer.get() << std::endl;

        // Create game input object
        gameInput = std::make_unique<RemoteGameInput>(inputs);
        std::cout << "DEBUG: gameInput created at " << gameInput.get() << std::endl;

        // Ensure pointers are valid before passing to RemoteListenerContext
        IInputWithTimer* pairingPtr = pairingInputWithTimer.get();
        IInputWithTimer* noBlinkPtr = noBlinkInputWithTimer.get();
        IInputWithTimer* sleepingPtr = sleepingInputWithTimer.get();
        IGameInput* gameInputPtr = gameInput.get();

        std::cout << "DEBUG: Before creating context:" << std::endl;
        std::cout << "DEBUG: pairingPtr = " << pairingPtr << std::endl;
        std::cout << "DEBUG: noBlinkPtr = " << noBlinkPtr << std::endl;
        std::cout << "DEBUG: sleepingPtr = " << sleepingPtr << std::endl;
        std::cout << "DEBUG: gameInputPtr = " << gameInputPtr << std::endl;

        std::cout << "DEBUG: pairingInputWithTimer (before passing to context) = " 
                << pairingInputWithTimer.get() << std::endl;
        std::cout << "DEBUG: noBlinkInputWithTimer (before passing to context) = " 
                << noBlinkInputWithTimer.get() << std::endl;
        std::cout << "DEBUG: sleepingInputWithTimer (before passing to context) = " 
                << sleepingInputWithTimer.get() << std::endl;
        std::cout << "DEBUG: gameInput (before passing to context) = " 
                << gameInput.get() << std::endl;


        bool noScoreFlag = true;
        context = std::make_unique<RemoteListenerContext>(
            scoreboard, gameObject, gameState, reset, remoteLocker, 
            pairingPtr, noBlinkPtr, sleepingPtr, gameInputPtr, 
            remotePairingScreen.get(), pairingBlinker, blankBlinker, 
            sleepingBlinker, noScoreFlag
        );

        std::cout << "DEBUG: RemoteListenerContext created at " << context.get() << std::endl;

        // Finally, create the pairing mode state
        pairingModeState = std::make_unique<PairingModeState>();
        std::cout << "DEBUG: pairingModeState created at " << pairingModeState.get() << std::endl;
    }

    void TearDown() override {
        pairingModeState.reset();
        context.reset();
        remotePairingScreen.reset();
        gameInput.reset();
        sleepingInputWithTimer.reset();
        noBlinkInputWithTimer.reset();
        pairingInputWithTimer.reset();
        inputs->~Inputs();
        delete remoteLocker;
        delete reset;
        delete gameObject;
        delete gameState;
    }
};

TEST_F(PairingModeStateTest, ConstructDestruct) {
    std::cout << "DEBUG: Entering PairingModeStateTest.cpp" << std::endl;
    EXPECT_NO_THROW(PairingModeState testInstance;);
}

TEST_F(PairingModeStateTest, HandleInput_ValidTransition) {
    std::cout << "DEBUG: pairingInputWithTimer = " << context->getPairingInputWithTimer() << std::endl;
    EXPECT_NO_THROW(pairingModeState->handleInput(*context));
}

TEST_F(PairingModeStateTest, HandleInput_StressTest) {
    std::cout << "DEBUG: pairingInputWithTimer = " << context->getPairingInputWithTimer() << std::endl;
    for (int i = 0; i < 10; ++i) {
        EXPECT_NO_THROW(pairingModeState->handleInput(*context));
    }
}
