/*
 * Author: Arne
 * Version: 0.2
 * Kommentar: Wer noch irgendwo Static statt Dynamic findet bekommt n Eis von mir, lg Arne
 *
 * Author: Lucas
 * Version: 0.3
 * Datum: 12.01.17
 * Kommentar: Fehlerbehebung
 *
 * Author: Lucas, Manuel
 * Version: 0.4
 * Datum: 13.01.2018
 */
#include "dynamicunit.h"
#include "staticunit.h"

#include <QDebug>
#include <QTime>
#include <QFile>
#include <QTextStream>
#include <QPainter>
//DynamicUnit

/*
Konstruktor zum initalisieren einer dynamischen Einheit
*/
DynamicUnit::DynamicUnit(QString filepath, Player* player)
    : Unit(), int_unitLevel(0)
{
    QFile file(filepath);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Warnung: File nicht gefunden \n\t Pfad: " << filepath;
        return;
    }
    QTextStream in(&file);
	
    str_unitName = in.readLine();
	in >> int_unitView;
    in >> int_actionRange;
	in >> int_unitHP;
	in >> int_unitStorageMax;
    in >> int_unitCost;
    in >> int_unitMoveRange;
    in >> int_unitAirAtt;
    in >> int_unitWaterAtt;
    in >> int_unitGroundAtt;
    in >> int_unitAutoRep;
	in >> str_unitType;

    QString buffer; //Aufgrund des Zeilenumbruchs
    buffer = in.readLine();
    str_unitDetails = in.readLine();

    QString pixmapPathPlayerOne = in.readLine();
    QString pixmapPathPlayerTwo = in.readLine();
    QPixmap pix;
    if(player == nullptr)
    {
        pix.load(pixmapPathPlayerOne);
    }else if(player->getPlayerID() == 1)
    {
        pix.load(pixmapPathPlayerOne);
    }else if(player->getPlayerID() == 2)
    {
        pix.load(pixmapPathPlayerTwo);
    }

    setPixmap(pix.scaled(64,64));

    int_unitCurrentHP = int_unitHP;
    int_unitCurrentMoveRange = int_unitMoveRange;
    unitFile = filepath;

    setUnitPlayer(player);

    if(int_unitView < int_unitMoveRange)
    {
        int_unitView = int_unitMoveRange;
    }
}

/*
Diese Funktion dient zur grafischen Alternierung benutzter Einheiten
*/
void DynamicUnit::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(bool_unitUsed == true || int_unitCurrentMoveRange == 0)
    {
        painter->setOpacity(.7);
    }
    QGraphicsPixmapItem::paint(painter,option,widget);

    QPen pen;
    QBrush brush;

    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::red);
    pen.setWidth(1);
    pen.setColor(Qt::red);

    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRect(QRect(19,5,25,3));

    if(int_unitCurrentHP > 0)
    {
        brush.setColor(Qt::green);
        pen.setWidth(1);
        pen.setColor(Qt::green);

        painter->setPen(pen);
        painter->setBrush(brush);
        painter->drawRect(QRect(19,5,(double)int_unitCurrentHP / (double)int_unitHP * 25,3));
    }
}

/*
Serialize ist eine Hilfsfunktion zur Speicherung von Einheitenzuständen
*/
void DynamicUnit::serialize(QTextStream & out)
{
    out << str_unitType << " " << unitFile << " " << unitPlayer->getPlayerID() << " "
        << int_unitCurrentHP << " " << bool_unitUsed  << " " << int_unitLevel << " "
        << int_unitCurrentMoveRange << "\n";
}

/*
Funktion zum reparieren von Einheiten am Ende einer Runde
*/
void DynamicUnit::autoRepair() {
	if (int_unitCurrentHP + int_unitAutoRep > int_unitHP) {
		int_unitCurrentHP = int_unitHP;
	}

	else {
		int_unitCurrentHP = int_unitCurrentHP + int_unitAutoRep;
	}
}

/*
Diese Funktion levelt eine Einheit alle 3 Kills. Dabei erhält die Einheit eine Verstärkung aller Angriffswerte
*/
void DynamicUnit::levelUpBonus()
{
    //setze Level alle 3 Kills eins höher
    if (int_unitEXP == 30)
    {
        double percentage = int_unitEXP / 100.0;
        int_unitAirAtt = int_unitAirAtt*(1 + percentage);
        int_unitGroundAtt = int_unitGroundAtt*(1 + percentage);
        int_unitWaterAtt = int_unitWaterAtt*(1 + percentage);

        int_unitLevel += 1;
        int_unitEXP = 0;
    }
}

/*
In dieser Funktion werden nach Ende einer Runde die Einheiten wieder auf ungenutzt zurückgesetzt
*/
void DynamicUnit::resetUnit()
{
    int_unitCurrentMoveRange = int_unitMoveRange;
    bool_unitUsed = false;
    if((int_unitCurrentHP += int_unitAutoRep) > int_unitHP)
    {
        int_unitCurrentHP = int_unitHP;
    }
}

int DynamicUnit::getUnitAutoRep() const{
        return int_unitAutoRep;
}

int DynamicUnit::getUnitLevel() const{
        return int_unitLevel;
}

void DynamicUnit::setUnitAutoRep(const int newUnitAutoRep)
{
    int_unitAutoRep = newUnitAutoRep;
}

void DynamicUnit::setUnitLevel(const int value)
{
    int_unitLevel = value;
}




// AirUnit

/*
Konstruktor für die Airunits
*/
AirUnit::AirUnit(QString filepath, Player* player)
    : DynamicUnit(filepath, player){}

/*
Diese Funktion legt die Bewegungsfreiheit von den AirUnits fest
*/
int AirUnit::moveTo(HexagonMatchfield *hex_target)
{
    //Flugzeug hat selbe Kosten für alles.
    if(hex_target->getUnitStationed() == nullptr)
    {
        return 1;
    }else if(hex_target->getUnitStationed()->getUnitPlayer() == unitPlayer
            && hex_target->getUnitStationed()->getUnitType().contains("TRANSPORTER")){
        return 1;
    }
    return -1;
}

/*
In dieser Funktion wird eine ausgewählte Einheit (wenn vorhanden), welche auf einem Hexagon liegt, von einer AirUnit angegriffen
*/
bool AirUnit::action(HexagonMatchfield *hex_target, const int range) {

    Unit *target = hex_target->getUnitStationed();

    if(bool_unitUsed || target == nullptr || target->getUnitPlayer() == this->getUnitPlayer())
    {
        return false;
    }

    QString target_type = target->getUnitType();

    //Für eine Zufallszahl
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    int int_unitSpecificAtt = 0;

    if(target_type.contains("WATER"))
    {
        int_unitSpecificAtt = int_unitWaterAtt;
    } else if(target_type.contains("AIR"))
    {
        int_unitSpecificAtt = int_unitAirAtt;
    } else {
        int_unitSpecificAtt = int_unitGroundAtt;
    }

    //Angriff
    target->setUnitCurrentHP(target->getUnitCurrentHP() - int_unitSpecificAtt);

    if(target->getUnitCurrentHP() > 0 && range <= target->getActionRange())
    {
        //zurück angreifen mit 50 - 75% unseres Attk. Wertes
        double randomPercentage = (qrand() % 26 + 50) / 100.0;

        //Gegenangriff
        int_unitCurrentHP -= target->getUnitAirAtt() * randomPercentage;
        if(int_unitCurrentHP <= 0)
        {
            target->setUnitEXP(target->getUnitEXP() + 10);
            target->levelUpBonus();
        }
    }else{
        int_unitEXP += 10;
        levelUpBonus();
    }
    bool_unitUsed = true;
    return true;
}

/*
Erstellung einer AirUnit in StaticUnits
*/
AirUnit *AirUnit::createUnit()
{
    return new AirUnit(unitFile, unitPlayer);
}
// TransporterUnit

/*
Konstruktor für alle TransporterUnits
*/
TransporterUnit::TransporterUnit(QString filepath, Player* player)
    : DynamicUnit(filepath, player), ptr_unitToUnload(nullptr){}

int TransporterUnit::getTransporterUnitCapacity() const{
		return int_transporterUnitCapacity;
}
int TransporterUnit::getTransporterUnitCurrentCapacity() const{
		return int_transporterUnitCurrentCapacity;
}
void TransporterUnit::setTransporterUnitCurrentCapacity(const int newCurrentCapacity)
{
	int_transporterUnitCurrentCapacity = newCurrentCapacity;
	return;
}

/*
Diese Funktion fungiert als Entscheidungsfunktion, welche bestimmte Aktion der Transporter durchführen wird
*/
bool TransporterUnit::action(HexagonMatchfield* hex_target, const int )
{
    if(bool_unitUsed == true)
    {
        return false;
    }
    if(hex_target->getUnitStationed() == nullptr)
    {
        if(ptr_unitToUnload != nullptr && ptr_unitToUnload->moveTo(hex_target) != -1)
        {
            unload(hex_target);
            resetBuildUnloadParameter();
            bool_unitUsed = true;
            return true;
        }
        else if(hex_target->getBoltaniumCurrent() > 0 ){
			farmBoltanium(hex_target);
            bool_unitUsed = true;
			return true;
		}
    }
    resetBuildUnloadParameter();
	return false;
}

/*
Diese Funktion fungiert als Ausladefunktion der in den TransporterUnits befindlichen DynamicUnits
*/
void TransporterUnit::unload(HexagonMatchfield* hex_target){
    hex_target->setUnitStationed(ptr_unitToUnload);
    disconnect(ptr_unitToUnload->getUnitDisplay(), SIGNAL(unitDispl_clicked(Unit*)), this, SLOT(SLOT_setUnitToUnload(Unit*)));
	
	int i = 0;
    for(auto &x : vector_unitStorage){
        if(ptr_unitToUnload == x){
			vector_unitStorage.erase(vector_unitStorage.begin() + i);
			break;
		}
		i++;
	}
}

/*
Diese Funktion nimmt mithilfe von TransporterUnits die Energieressource Boltanium auf und schreibt es dem Spielerenergieressourcenkonto zu  
*/
void TransporterUnit::farmBoltanium(HexagonMatchfield* hex_target){
    if(hex_target->getBoltaniumCurrent() >= 30){
        unitPlayer->setCurrentEnergieStorage(unitPlayer->getCurrentEnergieStorage() + 30);
        hex_target->setBoltaniumCurrent(hex_target->getBoltaniumCurrent() - 30);
	}
	
	else{
		unitPlayer->setCurrentEnergieStorage(unitPlayer->getCurrentEnergieStorage() + hex_target->getBoltaniumCurrent());
		hex_target->setBoltaniumCurrent(0);
    }
}

/*
Diese Funktion fungiert als Einladefunktion der TransporterUnits, welche DynamicUnits transportieren kann
*/
void TransporterUnit::addUnitToStorage(Unit *unit)
{
    vector_unitStorage.push_back(unit);
    connect(unit->getUnitDisplay(),SIGNAL(unitDispl_clicked(Unit*)),this, SLOT(SLOT_setUnitToUnload(Unit*)));
}

/*
Hilfsfunktion zum Speichern des aktuellen Spielstandes
*/
void TransporterUnit::serialize(QTextStream &out)
{
    DynamicUnit::serialize(out);
    out << vector_unitStorage.size() << "\n";
    for(auto &unitInStorage : vector_unitStorage)
    {
        unitInStorage->serialize(out);
    }
}

/*
Diese Funktion dient zur grafischen Alternierung benutzter BuilderEinheiten
*/
void TransporterUnit::resetBuildUnloadParameter()
{
    if(ptr_unitToUnload != nullptr)
    {
        for(auto &it : vector_unitStorage)
        {
            it->getUnitDisplay()->setColor(Qt::black);
            it->getUnitDisplay()->setZValue(0);
            ptr_unitToUnload = nullptr;
        }
    }
}

/*
SLOT für Qt Mechaniken
*/
void TransporterUnit::SLOT_setUnitToUnload(Unit *unit)
{
    resetBuildUnloadParameter();
    ptr_unitToUnload = unit;
}


// TransporterAirUnit

/*
spezifischer Konstruktor für AirTransporter
*/
TransporterAirUnit::TransporterAirUnit(QString filepath, Player* player)
    : TransporterUnit(filepath, player){}

/*
Diese Funktion legt die Bewegungsfreiheit von den TransporterAirUnits fest
*/
int TransporterAirUnit::moveTo(HexagonMatchfield *hex_target){
	//Flugzeug hat selbe Kosten für alles.
    if(hex_target->getUnitStationed() == nullptr)
    {
        return 1;
    }else if(hex_target->getUnitStationed()->getUnitPlayer() == unitPlayer
            && hex_target->getUnitStationed()->getUnitType().contains("TRANSPORTER")){
        return 1;
    }
    return -1;
}

/*
Erstellfunktion einer TransporterAirUnit
*/
TransporterAirUnit *TransporterAirUnit::createUnit()
{
    return new TransporterAirUnit(unitFile, unitPlayer);
}





// TransporterGroundUnit

/*
Konstruktor der TransporterGroundUnits
*/
TransporterGroundUnit::TransporterGroundUnit(QString filepath, Player* player)
    : TransporterUnit(filepath, player){}

/*
Diese Funktion legt die Bewegungsfreiheit von den TransporterGroundUnits fest
*/
int TransporterGroundUnit::moveTo(HexagonMatchfield *hex_target){

    QString hex_type = hex_target->getHexMatchfieldType();

    if(hex_target->getUnitStationed() == nullptr)
    {

        if(hex_type == "grassland"){
            return 1;
        }

        else if(hex_type.contains("street")){
            return 1;
        }

        else if(hex_type == "forrest" ){
            return 2;
        }

        else if(hex_type.contains("mountain")){
            return 2;
        }
    }else if(hex_target->getUnitStationed()->getUnitPlayer() == unitPlayer
            && hex_target->getUnitStationed()->getUnitType().contains("TRANSPORTER"))
    {
        return 1;
    }

    return -1;
}

/*
Erstellfunktion der TransporterGroundUnits
*/
TransporterGroundUnit *TransporterGroundUnit::createUnit()
{
    return new TransporterGroundUnit(unitFile, unitPlayer);
}




// TransporterWaterUnit

/*
Konstruktor der TransporterWaterUnits
*/
TransporterWaterUnit::TransporterWaterUnit(QString filepath, Player* player)
    : TransporterUnit(filepath, player){}

/*
Diese Funktion legt die Bewegungsfreiheit von den TransporterWaterUnits fest
*/
int TransporterWaterUnit::moveTo(HexagonMatchfield *hex_target){

    QString hex_type = hex_target->getHexMatchfieldType();
    if(hex_target->getUnitStationed() == nullptr)
    {
        if(hex_type == "waterDeep"){
            return 2;
        }
        else if(hex_type == "waterSeashore"){
            return 1;
        }
    }else if(hex_target->getUnitStationed()->getUnitPlayer() == unitPlayer
            && hex_target->getUnitStationed()->getUnitType().contains("TRANSPORTER")){
        return 1;
    }

    return -1;
}

/*
Erstellfunktion der TransporterWaterUnits
*/
TransporterWaterUnit *TransporterWaterUnit::createUnit()
{
    return new TransporterWaterUnit(unitFile, unitPlayer);
}



// GroundUnit

/*
Konstruktor der GroundUnits
*/
GroundUnit::GroundUnit(QString filepath, Player* player)
	: DynamicUnit(filepath, player){}

/*
In dieser Funktion wird eine ausgewählte Einheit (wenn vorhanden), welche auf einem Hexagon liegt, von einer GroundUnit angegriffen
*/
bool GroundUnit::action(HexagonMatchfield *hex_target, const int range) {
    if(bool_unitUsed)
    {
        return false;
    }

    Unit *target = hex_target->getUnitStationed();
	
	if(target == nullptr){
		return false;
	}

    if(target->getUnitPlayer() == this->getUnitPlayer())
    {
        return false;
    }

	int int_target_current_hp = target->getUnitCurrentHP();
    QString target_type = target->getUnitType();

	/* RNG mit zufälligem Seed für später initalisieren */
	srand(time(NULL));

	int int_unitSpecificAtt = 0;
	bool bool_fights_back = false;

	if(target_type == "LIGHTUNIT" || target_type == "MEDIUMUNIT" || target_type == "HEAVYUNIT"){
		int_unitSpecificAtt = int_unitGroundAtt; bool_fights_back = true;
	}

	else if(target_type == "WATERUNIT"){
		int_unitSpecificAtt = int_unitWaterAtt; bool_fights_back = true;
	}

	else if(target_type == "AIRUNIT"){
		int_unitSpecificAtt = int_unitAirAtt; bool_fights_back = true;
	}

	if(bool_fights_back && range <= target->getActionRange()){

		if (int_target_current_hp - int_unitSpecificAtt <= 0) {
			target->setUnitCurrentHP(0);
			int_unitEXP +=10;
			levelUpBonus();
            bool_unitUsed = true;
			return true;
		}

		else {
			target->setUnitCurrentHP(int_target_current_hp - int_unitSpecificAtt);

			//zurück angreifen mit 50 - 75% unseres Attk. Wertes
			int int_generated_number = (rand() % 26) + 50;
			double db_percentage_value = ((double)int_generated_number) / 100.0;

			int backfire = (int)(target->getUnitGroundAtt()*db_percentage_value);
			if(int_unitCurrentHP - backfire <= 0){
				int_unitCurrentHP = 0;
				target->setUnitEXP(target->getUnitEXP() + 10);
				target->levelUpBonus();
                bool_unitUsed = true;
				return true;
			}

			else{
				int_unitCurrentHP -= backfire;
                bool_unitUsed = true;
				return true;
			}

		}
	}

	if(!bool_fights_back){
		if(target_type == "TRANSPORTERAIR"){
			int_unitSpecificAtt = int_unitAirAtt;
		}

		else if(target_type == "TRANSPORTERWATER"){
			int_unitSpecificAtt = int_unitWaterAtt;
		}

		else if(target_type == "TRANSPORTERGROUND"){
			int_unitSpecificAtt = int_unitGroundAtt;
		}

		//Gebäude
		else{
			int_unitSpecificAtt = int_unitGroundAtt;
		}

		if (int_target_current_hp - int_unitSpecificAtt <= 0) {
			target->setUnitCurrentHP(0);
			int_unitEXP +=10;
			levelUpBonus();
            bool_unitUsed = true;
			return true;
		}

		else {
			target->setUnitCurrentHP(int_target_current_hp - int_unitSpecificAtt);
            bool_unitUsed = true;
			return true;
		}
	}
	return false;

}

// LightUnit

/*
Konstruktor der LightUnits
*/
LightUnit::LightUnit(QString filepath, Player* player)
	: GroundUnit(filepath, player){}

/*
Diese Funktion legt die Bewegungsfreiheit von den LightUnits fest
*/
int LightUnit::moveTo(HexagonMatchfield *hex_target){

    QString hex_type = hex_target->getHexMatchfieldType();
    if(hex_target->getUnitStationed() == nullptr)
    {
        if(hex_type == "grassland"){
            return 1;
        }

        else if(hex_type.contains("street")){
            return 1;
        }

        else if(hex_type == "forrest" ){
            return 1;
        }

        else if(hex_type.contains("mountain")){
            return 1;
        }
    }else if(hex_target->getUnitStationed()->getUnitPlayer() == unitPlayer
            && hex_target->getUnitStationed()->getUnitType().contains("TRANSPORTER")){
        return 1;
    }

    return -1;
}

/*
Erstellfunktion der LightUnits
*/
LightUnit *LightUnit::createUnit()
{
    return new LightUnit(unitFile, unitPlayer);
}

// BuildLightUnit

/*
Konstruktor der BuildLightUnit
*/
BuildLightUnit::BuildLightUnit(QString filepath, bool bool_loadInventory, Player* player)
    : LightUnit(filepath, player), qstring_unitToBuild("")
{
    if(bool_loadInventory == true)
    {
        production["Depot"] = new DepotUnit(":/static/staticUnit/depot.txt", unitPlayer);
        production["Fabrik"] = new FactoryUnit(":/static/staticUnit/factory.txt", false, unitPlayer);

        for(auto &it : production)
        {
            connect(it.second->getUnitDisplay(), SIGNAL(unitDispl_clicked(Unit*)), this, SLOT(SLOT_setUnitToBuild(Unit*)));
            vector_unitStorage.push_back(it.second);
        }
    }
}

/*
Destruktor einer BuildLightUnit
*/
BuildLightUnit::~BuildLightUnit()
{
    production.clear();
}

/*
Diese Funktion entscheidet, ob die BuildLightUnit etwas baut oder nicht 
*/
bool BuildLightUnit::action(HexagonMatchfield* hexTarget, const int){
	
    if(bool_unitUsed == true)
    {
        return false;
    }
    if(qstring_unitToBuild != "" && unitPlayer->getCurrentEnergieStorage() - production[qstring_unitToBuild]->getUnitCost() >= 0
            && hexTarget->getUnitStationed() == nullptr && production[qstring_unitToBuild]->moveTo(hexTarget) != -1)
    {
        produceUnit(hexTarget);
        resetBuildUnloadParameter();
        bool_unitUsed = true;
        return true;
    }
    resetBuildUnloadParameter();
    return false;
}

/*
Diese Funktion setzt auf das ausgewählte Hexagon eine StaticUnit
*/
void BuildLightUnit::produceUnit(HexagonMatchfield* hexTarget)
{
    hexTarget->setUnitStationed(production[qstring_unitToBuild]->createUnit());
    hexTarget->getUnitStationed()->setUnitCost(0);
    hexTarget->getUnitStationed()->setUnitUsed(true);
    unitPlayer->setCurrentEnergieStorage(unitPlayer->getCurrentEnergieStorage() - hexTarget->getUnitStationed()->getUnitCost());
}

/*
Hilfsfunktion zur Erstellung einer BuildLightUnit
*/
BuildLightUnit *BuildLightUnit::createUnit()
{
    return new BuildLightUnit(unitFile, true, unitPlayer);
}

/*
Zu bauende Einheit zurücksetzen
*/
void BuildLightUnit::resetBuildUnloadParameter()
{
    if(qstring_unitToBuild != "")
    {
        production[qstring_unitToBuild]->getUnitDisplay()->setColor(Qt::black);
        production[qstring_unitToBuild]->getUnitDisplay()->setZValue(0);
        qstring_unitToBuild = "";
    }
}

//für die KI
void BuildLightUnit::setUnitToBuild(QString name)
{
    qstring_unitToBuild = name;
}

/*
Hilfsfunktion für QWt Mechaniken
*/
void BuildLightUnit::SLOT_setUnitToBuild(Unit *unit)
{
    resetBuildUnloadParameter();
    qstring_unitToBuild = unit->getUnitName();
}


// MediumUnit

/*
Konstruktor für MediumUnits
*/
MediumUnit::MediumUnit(QString filepath, Player* player)
	: GroundUnit(filepath, player){}

/*
Diese Funktion legt die Bewegungsfreiheit von den MediumUnits fest
*/
int MediumUnit::moveTo(HexagonMatchfield *hex_target){

    QString hex_type = hex_target->getHexMatchfieldType();
    if(hex_target->getUnitStationed() == nullptr)
    {
        if(hex_type == "grassland"){
            return 1;
        }

        else if(hex_type.contains("street")){
            return 1;
        }

        else if(hex_type == "forrest" ){
            return 2;
        }

        else if(hex_type.contains("mountain") ){
            return -1;
        }
    }else if(hex_target->getUnitStationed()->getUnitPlayer() == unitPlayer
            && hex_target->getUnitStationed()->getUnitType().contains("TRANSPORTER")){
        return 1;
    }

    return -1;

}
/*
Hilfsfunktion zum Erstellen einer MediumUnit
*/
MediumUnit *MediumUnit::createUnit()
{
    return new MediumUnit(unitFile, unitPlayer);
}
// HeavyUnit

/*
Konstruktor der HeavyUnits
*/
HeavyUnit::HeavyUnit(QString filepath, Player* player)
	: GroundUnit(filepath, player){}

/*
Diese Funktion legt die Bewegungsfreiheit von den HeavyUnits fest
*/
int HeavyUnit::moveTo(HexagonMatchfield *hex_target){

    QString hex_type = hex_target->getHexMatchfieldType();
    if(hex_target->getUnitStationed() == nullptr)
    {
        if(hex_type == "grassland"){
            return 2;
        }

        else if(hex_type.contains("street")){
            return 1;
        }

        else if(hex_type == "forrest" ){
            return -1;
        }

        else if(hex_type.contains("mountain") ){
            return -1;
        }
    }else if(hex_target->getUnitStationed()->getUnitPlayer() == unitPlayer
            && hex_target->getUnitStationed()->getUnitType().contains("TRANSPORTER")){
        return 1;
    }
    return -1;
}

/*
Hilfsfunktion zum Erstellen einer HeavyUnit
*/
HeavyUnit *HeavyUnit::createUnit()
{
    return new HeavyUnit(unitFile, unitPlayer);
}


// WaterUnit

/*
Konstruktor der WaterUnits
*/
WaterUnit::WaterUnit(QString filepath, Player* player)
	: DynamicUnit(filepath, player){}

/*
Diese Funktion legt die Bewegungsfreiheit von den WaterUnits fest
*/
int WaterUnit::moveTo(HexagonMatchfield *hex_target){

    QString hex_type = hex_target->getHexMatchfieldType();
    if(hex_target->getUnitStationed() == nullptr)
    {
        if(hex_type == "waterDeep"){
            return 1;
        }
        else if(hex_type == "waterSeashore"){
            return 2;
        }
    }else if(hex_target->getUnitStationed()->getUnitPlayer() == unitPlayer
            && hex_target->getUnitStationed()->getUnitType().contains("TRANSPORTER")){
        return 1;
    }

    return -1;

}


/*
In dieser Funktion wird eine ausgewählte Einheit (wenn vorhanden), welche auf einem Hexagon liegt, von einer WaterUnit angegriffen
*/
bool WaterUnit::action(HexagonMatchfield *hex_target, const int range) {
    if(bool_unitUsed)
    {
        return false;
    }

    Unit *target = hex_target->getUnitStationed();
	
	if(target == nullptr){
		return false;
	}

    if(target->getUnitPlayer() == this->getUnitPlayer())
    {
        return false;
    }

	int int_target_current_hp = target->getUnitCurrentHP();
    QString target_type = target->getUnitType();

	/* RNG mit zufälligem Seed für später initalisieren */
	srand(time(NULL));

	int int_unitSpecificAtt = 0;
	bool bool_fights_back = false;

	if(target_type == "LIGHTUNIT" || target_type == "MEDIUMUNIT" || target_type == "HEAVYUNIT"){
		int_unitSpecificAtt = int_unitGroundAtt; bool_fights_back = true;
	}

	else if(target_type == "WATERUNIT"){
		int_unitSpecificAtt = int_unitWaterAtt; bool_fights_back = true;
	}

	else if(target_type == "AIRUNIT"){
		int_unitSpecificAtt = int_unitAirAtt; bool_fights_back = true;
	}

	if(bool_fights_back &&  range <= target->getActionRange()){

		if (int_target_current_hp - int_unitSpecificAtt <= 0) {
			target->setUnitCurrentHP(0);
			int_unitEXP +=10;
			levelUpBonus();
            bool_unitUsed = true;
			return true;
		}

		else {
			target->setUnitCurrentHP(int_target_current_hp - int_unitSpecificAtt);

			//zurück angreifen mit 50 - 75% unseres Attk. Wertes
			int int_generated_number = (rand() % 26) + 50;
			double db_percentage_value = ((double)int_generated_number) / 100.0;

			int backfire = (int)(target->getUnitWaterAtt()*db_percentage_value);
			if(int_unitCurrentHP - backfire <= 0){
				int_unitCurrentHP = 0;
				target->setUnitEXP(target->getUnitEXP() + 10);
				target->levelUpBonus();
                bool_unitUsed = true;
				return true;
			}

			else{
				int_unitCurrentHP -= backfire;
                bool_unitUsed = true;
				return true;
			}

		}
	}

	if(!bool_fights_back){
		if(target_type == "TRANSPORTERAIR"){
			int_unitSpecificAtt = int_unitAirAtt;
		}

		else if(target_type == "TRANSPORTERWATER"){
			int_unitSpecificAtt = int_unitWaterAtt;
		}

		else if(target_type == "TRANSPORTERGROUND"){
			int_unitSpecificAtt = int_unitGroundAtt;
		}

		//Gebäude
		else{
			int_unitSpecificAtt = int_unitGroundAtt;
		}

		if (int_target_current_hp - int_unitSpecificAtt <= 0) {
			target->setUnitCurrentHP(0);
			int_unitEXP +=10;
			levelUpBonus();
            bool_unitUsed = true;
			return true;
		}

		else {
			target->setUnitCurrentHP(int_target_current_hp - int_unitSpecificAtt);
            bool_unitUsed = true;
			return true;
		}
	}
	return false;

}

/*
Hilfsfunktion zur Erstellung einer WaterUnit
*/
WaterUnit *WaterUnit::createUnit()
{
    return new WaterUnit(unitFile, unitPlayer);
}
