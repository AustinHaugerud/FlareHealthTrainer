// FlareTrainer.cpp : Defines the entry point for the console application.
//

#include"Trainer.hpp"
#include<iostream>



int main()
{
	Trainer trainer;

	/*
	 * Player health will be locked to 100hp
	 */
	trainer.setHealthValue(100);

	/*
	 * Player health will be updated every 100ms
	 */
	trainer.setUpdateFrequency(100);
	
	if (trainer.run() == 1)
	{
		std::cerr << "Errors occured" << std::endl;
		std::cout << "Errors occured" << std::endl;
	}
}
