#include "ScoreBoard.h"
#include "Arduino.h"

ScoreBoard::ScoreBoard( PinInterface* pinInterface, Rules* rules ) : _pinInterface( pinInterface ), 
                                                                     _rules(        rules ){}
ScoreBoard::~ScoreBoard() {}

void ScoreBoard::showSetWinner( Team* scoring_team, Team* opposing_team ) {
    for ( int i = 0; i < SET_WINNER_FLASH_COUNT; i++ ) {
        setTeamASets( scoring_team->number()   == TEAM_A ? scoring_team->getSets()    : opposing_team->getSets()  );
        setTeamBSets( opposing_team->number()  == TEAM_B ? opposing_team->getSets()   : scoring_team->getSets()   );
        setTeamAPoints( scoring_team->number() == TEAM_A ? scoring_team->getPoints()  : opposing_team->getPoints());
        setTeamBPoints( scoring_team->number() == TEAM_B ? scoring_team->getPoints()  : opposing_team->getPoints());
        GameTimer::gameDelay( SET_WINNER_FLASH_DELAY );
        scoring_team->number() == TEAM_A ? setTeamASets( 0 ) : setTeamBSets( 0 ); // blink               
        GameTimer::gameDelay( SET_WINNER_FLASH_DELAY ); }
}

void ScoreBoard::showMatchWinner( Team* scoring_team, Team* opposing_team ) {
        ShowMatchWin showMatchWin( scoring_team, opposing_team, this ); showMatchWin.execute(); }

void ScoreBoard::update( Team* scoring_team, Team* opposing_team ) {
    setTeamAPoints( scoring_team->number()  == TEAM_A ? scoring_team->getPoints()  : opposing_team->getPoints() ); 
    setTeamBPoints( opposing_team->number() == TEAM_B ? opposing_team->getPoints() : scoring_team->getPoints()  );
    setTeamASets(   scoring_team->number()  == TEAM_A ? scoring_team->getSets()    : opposing_team->getSets()   );
    setTeamBSets(   opposing_team->number() == TEAM_B ? opposing_team->getSets()   : scoring_team->getSets()    );
    setTeamAServe(  scoring_team->number()  == TEAM_A ? scoring_team->getServe()   : opposing_team->getServe()  );
    setTeamBServe(  opposing_team->number() == TEAM_B ? opposing_team->getServe()  : scoring_team->getServe()   ); }

void ScoreBoard::setTeamAPoints( int points ) {
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_0,  points == 1 || points == 12 ? 1 : 0 );
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_1,  points == 2 || points == 13 || points == 23 ? 1 : 0 );
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_2,  points == 3 || points == 14 || points == 24 ? 1 : 0 );
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_3,  points == 4 || points == 15 ? 1 : 0 );
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_4,  points == 5 || points == 16 ? 1 : 0 );
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_5,  points == 6 || points == 17 ? 1 : 0 );
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_6,  points == 7 || points == 18 ? 1 : 0 );
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_7,  points == 8 || points == 19 ? 1 : 0 );
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_8,  points == 9 || points == 20 ? 1 : 0 );
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_9,  points == 10 || ( points >= 21 && points < 99 ) ? 1 : 0 );
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_10, points >= 11 && points < 99 ? 1 : 0 ); }

void ScoreBoard::setTeamBPoints( int points ) {
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_0,  points == 1  || points == 12 ? 1 : 0 );
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_1,  points == 2  || points == 13 || points == 23 ? 1 : 0 );
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_2,  points == 3  || points == 14 || points == 24 ? 1 : 0 );
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_3,  points == 4  || points == 15 ? 1 : 0 );
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_4,  points == 5  || points == 16 ? 1 : 0 );
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_5,  points == 6  || points == 17 ? 1 : 0 );
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_6,  points == 7  || points == 18 ? 1 : 0 );
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_7,  points == 8  || points == 19 ? 1 : 0 );
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_8,  points == 9  || points == 20 ? 1 : 0 );
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_9,  points == 10  || ( points >= 21 && points < 99 ) ? 1 : 0 );
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_10, points >= 11 && points < 99  ? 1 : 0 ); }

void ScoreBoard::setTeamASets( int sets ) {
    _pinInterface->pinDigitalWrite( A_SET_1,  ( sets == 1 || sets == 2          ) ? 1 : 0 );
    _pinInterface->pinDigitalWrite( A_SET_2,  ( sets == 2 || sets == SET_2_ONLY ) ? 1 : 0 ); }

void ScoreBoard::setTeamBSets( int sets ) {
    _pinInterface->pinDigitalWrite( B_SET_1,  ( sets == 1 || sets == 2          ) ? 1 : 0 );
    _pinInterface->pinDigitalWrite( B_SET_2,  ( sets == 2 || sets == SET_2_ONLY ) ? 1 : 0 ); }

void ScoreBoard::setTeamAServe( int serve ) {
    _pinInterface->pinDigitalWrite( TEAM_A_SERVE_1, serve ==   _rules->getFreshServes()                     ? 1 : 0 );
    _pinInterface->pinDigitalWrite( TEAM_A_SERVE_2, serve == ( _rules->getFreshServes() - 1 ) && serve != 0 ? 1 : 0 ); }

void ScoreBoard::setTeamBServe( int serve ) {
    _pinInterface->pinDigitalWrite( TEAM_B_SERVE_1, serve ==   _rules->getFreshServes()                     ? 1 : 0 );
    _pinInterface->pinDigitalWrite( TEAM_B_SERVE_2, serve == ( _rules->getFreshServes() - 1 ) && serve != 0 ? 1 : 0 ); }

void ScoreBoard::toggleServe( Team* scoring_team, Team* opposing_team ) {
    if( scoring_team->getServe() > 0 ) {
        setTeamAServe( scoring_team->getServe() );
        setTeamBServe( 0                        );
    } else {
        setTeamBServe( opposing_team->getServe());
        setTeamAServe( 0                        ); }}

void ScoreBoard::setTeamAPointsOn() {
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_0,  1 );
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_1,  1 );
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_2,  1 );
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_3,  1 );
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_4,  1 );
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_5,  1 );
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_6,  1 );
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_7,  1 );
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_8,  1 );
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_9,  1 );
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_10, 1 ); }

void ScoreBoard::setTeamBPointsOn() {
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_0,  1 );
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_1,  1 );
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_2,  1 );
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_3,  1 );
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_4,  1 );
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_5,  1 );
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_6,  1 );
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_7,  1 );
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_8,  1 );
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_9,  1 );
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_10, 1 ); }