#include "controller.h"

Controller::Controller()
{
	//ofc = OneFingerCommand();
	tfc = TwoFingerCommand();
	//thfc = ThreeFingerCommand();
	//ffc = FourFingerCommand();
	//commands.insert( pair<fingersNumber,ICommand*>(ONE, &ofc));
	commands.insert( pair<fingersNumber,ICommand*>(TWO, &tfc));
	//commands.insert( pair<fingersNumber,ICommand*>(THREE, &thfc));
	//commands.insert( pair<fingersNumber,ICommand*>(FOUR, &ffc));
}

String Controller::recognize(Mat mat)
{
	Image im = Image(mat);
	int fingers = im.countFingers();
	if(fingers > 4 || fingers < 1)
		return "";

	return (commands.find(fingersNumber(fingers))->second->recognize(im));
}

void Controller::addGesture(Mat mat, String name)
{
	Image im = Image(mat);
	int fingers = im.countFingers();
	if(fingers > 4 || fingers < 1)
		return;

	commands.find(fingersNumber(fingers))->second->addGesture(im, name);
}
