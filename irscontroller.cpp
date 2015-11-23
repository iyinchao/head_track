#include "irscontroller.h"

IRSController::IRSController()
{
    sm = PXCSenseManager::CreateInstance();
}

void IRSController::stop()
{

}

void IRSController::start()
{

}

void IRSController::getSenseManager()
{
    return sm;
}

void IRSController::run()
{

}

