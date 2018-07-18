/*
 * Author: Miguel
 * Version: 0.3
 * Datum 25.11.2017
 * Kommentare: es gibt wie über whatsapp besprochen nur noch changePhase() 0.3 -> runde in 10er schritten
 * */
/*
 * Author: Lucas
 * Version: 0.4
 * Datum 14.01.2018
 * Kommentare: getCurrentPhase() eingefügt
 * */

#ifndef ROUND_HPP
#define ROUND_HPP

#include "enumerations.h"
#include <QTextStream>

class Phase;

class Round
{
private:
	int int_maxRoundNumber;
	int int_currentRoundNumber;
    Phase* phase_currentRoundPhase;

public:
	Round(int);
    Round(QString, int, int);
	~Round();
    void serialize(QTextStream&);
    static Round* unserialize(QTextStream&);
	void setCurrentRoundPhase(Phase*);
	void changePhase();
    bool checkGameOver();
	int getCurrentRoundNumber() const;
	void setCurrentRoundNumber(const int);
	int getMaxRoundNumber() const;
    PHASE getCurrentPhase() const;
};

class Phase
{
public:
	virtual void changePhase(Round*) = 0;
    virtual PHASE currentPhase() = 0;

    virtual ~Phase() = default;
};

class MovePhase : public Phase
{
public:
    void changePhase(Round*);
    PHASE currentPhase();
};

class ActionPhase : public Phase
{
public:
    void changePhase(Round*);
    PHASE currentPhase();
};

#endif
