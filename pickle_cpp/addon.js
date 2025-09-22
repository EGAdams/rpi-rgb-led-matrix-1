var pickle_cpp = require('bindings')('pickle_cpp');
console.log( "started..." );

let iterations = 0;
const PLAYER_BUTTONS = 202; 
console.log( "creating cpp object..." );
var cppInterface = new pickle_cpp.CppInterface( 42 );
console.log( "calling 1st digitalWrite()..." );
//cppInterface.digitalWrite( 38,  1    );  // RESET
//cppInterface.digitalWrite( 26,  1    );  // UNDO
cppInterface.digitalWrite( PLAYER_BUTTONS, 2000 );  // PLAYER_BUTTONS
let temp = cppInterface.getPinMap();
console.table( temp );
while ( iterations < 100 ) {
    if ( cppInterface.digitalRead( PLAYER_BUTTONS ) < 2000 ) {
        console.log( "pin PLAYER_BUTTONS is low!" ); }
    console.log( "calling gameLoop()..." );    
    cppInterface.gameLoop();
    console.log( "iteration: " + ++iterations );
    // postMessage( "iteration: " + iterations );
    temp = cppInterface.getPinMap();
    console.table( temp );  
}  

