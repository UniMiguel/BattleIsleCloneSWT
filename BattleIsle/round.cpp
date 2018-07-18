/*
 * Author: Miguel
 * Version: 0.3
 * Datum 20.12.2017
 * */
/*
 * Author: Lucas
 * Version: 0.4
 * Datum 14.01.2018
 * Kommentare: getCurrentPhase() eingefügt
 * */

#include "round.h"

Round::Round(int maxAnz = 0)
{
    int_maxRoundNumber=maxAnz*10;
    int_currentRoundNumber=10;
    phase_currentRoundPhase = new MovePhase;
    return;
}

Round::Round(QString phase, int currentNumber, int maxNumber)
{
    int_maxRoundNumber = maxNumber;
    int_currentRoundNumber = currentNumber;
    if(phase == "MOVE")
        phase_currentRoundPhase = new MovePhase;
    else
        phase_currentRoundPhase = new ActionPhase;
}

Round::~Round()
{
    delete phase_currentRoundPhase;
    return;
}

void Round::serialize(QTextStream & out)
{
    out << int_currentRoundNumber << " ";
    if(phase_currentRoundPhase->currentPhase() == MOVE)
    {
        out << "MOVE ";
    }else{
        out << "ACTION ";
    }
    out << int_maxRoundNumber;
    out << "\n";
}

Round *Round::unserialize(QTextStream &in)
{
    int currentRoundNumber, maxRoundNumber;
    QString str_round;

    in >> currentRoundNumber;
    in >> str_round;
    in >> maxRoundNumber;

    //qDebug() << "unserialize von Round ausfuehren mit \n\t" << currentRoundNumber << "\n\t" <<
    //            str_round << "\n\t" << maxRoundNumber;

    return new Round(str_round,currentRoundNumber,maxRoundNumber);
}

void Round::setCurrentRoundPhase(Phase* phase)
{
    delete phase_currentRoundPhase;
    phase_currentRoundPhase = phase;
	return;
}

void Round::changePhase()
{
    phase_currentRoundPhase->changePhase(this);
	return;
}

bool Round::checkGameOver()
{
	if(int_currentRoundNumber >= int_maxRoundNumber && int_maxRoundNumber != 0)
	{
		return true;
	}
	return false;
}

int Round::getCurrentRoundNumber() const
{
	return int_currentRoundNumber;
}

int Round::getMaxRoundNumber() const
{
    return int_maxRoundNumber;
}

PHASE Round::getCurrentPhase() const
{
    return phase_currentRoundPhase->currentPhase();
}

void Round::setCurrentRoundNumber(const int roundAnz)
{
	int_currentRoundNumber = roundAnz;
	return;
}

void MovePhase::changePhase(Round* round)
{
    round->setCurrentRoundPhase(new ActionPhase);
    return;
}

PHASE MovePhase::currentPhase()
{
    return MOVE;
}

void ActionPhase::changePhase(Round* round)
{
    round->setCurrentRoundPhase(new MovePhase);
    round->setCurrentRoundNumber(round->getCurrentRoundNumber() + 5);
}

PHASE ActionPhase::currentPhase()
{
    return ACTION;
}
