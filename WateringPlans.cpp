/***** #includes here. *****/

#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;

/***** Define constants here. *****/

const int UNITS_PER_CAN = 3;

int plant_count;            // count of plants
int plant_number;           // plant #1, #2, #3, ...
int unwatered_count;        // count of unwatered plants
int water_amount;           // amount of water in the watering can
int steps;                  // number of steps taken
int cum_steps;              // cumulative number of steps taken for a plan
int cum_step_units;         // cumulative step-units for each plan
int cum_step_units_near;    // cumulative step-units for Plan Near
int cum_step_units_far;     // cumulative step-units for Plan Far
bool trip_done;             // true if a trip is done

const string INPUT_FILE_NAME = "counts.txt";

/***** Declare functions here. *****/

void plan_title(string type, int count);

void walk_to_the_nearest_unwatered_plant_with_a_full_can();

void water_a_plant_while_walking_away_from_the_faucet();

void walk_away_from_the_faucet_to_the_next_unwatered_plant();

void walk_back_to_the_faucet_with_an_empty_or_partially_filled_can();

void walk_to_the_farthest_unwatered_plant_with_a_full_can();

void water_a_plant_while_walking_back_towards_the_faucet();

void walk_towards_the_faucet_to_the_next_unwatered_plant();

void plan_near();

void plan_far();

void better_plan();

/**
 * The main. For each count of plants, simulate Plan Near and Plan Far,
 * and decide which plan was better.
 */

int main()
{
	ifstream input;
	input.open(INPUT_FILE_NAME);
	if (input.fail())
	{
		cout << "Failed to open " << INPUT_FILE_NAME << endl;
		return -1;
	}

	// Loop once for each number of plants read from the input file.
	// Stop when the number of plants is 0 or less.
	// During each loop, simulate Plan Near and Plan Far.
	do
    {
       input >> plant_count;
 	    if (plant_count <= 0) return 0;

 	    plan_near();
 	    cum_step_units_near = cum_step_units;

 	    plan_far();
 	    cum_step_units_far = cum_step_units;

 	    better_plan();

    } while (plant_count > 0);

    return 0;
}

/***** Define functions here. *****/

void plan_title(string type, int count)
{
    cout << endl;
    cout << "==========================" << endl;
    cout << " Plan " << type << " with " << count << " plants" << endl;
    cout << "==========================" << endl;
    cout << endl;
    cout << "Where      Cum. steps  Water amt.  Cum. step-units" << endl;
    cout << "---------  ----------  ----------  ---------------" << endl;
}

void walk_to_the_nearest_unwatered_plant_with_a_full_can()
{
	plant_number = plant_count - unwatered_count + 1;
	steps = plant_number;
	cum_steps += steps;
	cum_step_units += steps*water_amount;
}

void water_a_plant_while_walking_away_from_the_faucet()
{
	cout << "Plant " << setw(2) << plant_number
     << setw(9)  << cum_steps
     << setw(11) << water_amount
     << setw(16) << cum_step_units << endl;
	water_amount--;
	unwatered_count--;
	trip_done = (water_amount == 0) || (unwatered_count == 0);
}

void walk_away_from_the_faucet_to_the_next_unwatered_plant()
{
    plant_number++;
    cum_steps++;
    cum_step_units += water_amount;
}

void walk_back_to_the_faucet_with_an_empty_or_partially_filled_can()
{
    steps = plant_number;
    cum_steps += steps;
    cum_step_units += steps*water_amount;

    cout << "FAUCET "
         << setw(10) << cum_steps
         << setw(11) << water_amount
         << setw(16) << cum_step_units  << endl;
}

void walk_to_the_farthest_unwatered_plant_with_a_full_can()
{
	plant_number = unwatered_count;
	steps = plant_number;
	cum_steps += steps;
	cum_step_units += steps*water_amount;
}

void water_a_plant_while_walking_back_towards_the_faucet()
{
    cout << "Plant " << setw(2) << plant_number
         << setw(9)  << cum_steps
         << setw(11) << water_amount
         << setw(16) << cum_step_units << endl;

    water_amount--;
    unwatered_count--;
    trip_done = (water_amount == 0) || (unwatered_count == 0);
}

void walk_towards_the_faucet_to_the_next_unwatered_plant()
{
	plant_number--;

	cum_steps++;
	cum_step_units += water_amount;
}

void better_plan()
{
    cout << endl;
    cout << "*** With " << plant_count << " plants, ";
    if (cum_step_units_far == cum_step_units_near)
    {
        cout << "both plans are equal." << endl;
    }
    else if (cum_step_units_far < cum_step_units_near)
    {
        cout << "Plan Far is better with "
             << cum_step_units_near - cum_step_units_far
             << " fewer step-units." << endl;
    }
    else
    {
        cout << "Plan Near is better with "
             << cum_step_units_far - cum_step_units_near
             << " fewer step-units." << endl;
    }
}

void reset()
{
    unwatered_count = plant_count;
    cum_steps = 0;
    cum_step_units = 0;
}

void plan_near()
{
    	    plan_title("Near", plant_count);
    	    reset();
        while (unwatered_count > 0)
        {
            // Fill the can.
            water_amount = UNITS_PER_CAN;

            walk_to_the_nearest_unwatered_plant_with_a_full_can();

            // Water plants while walking away from the faucet.
            // Loop once per plant until either the can is empty
            // or all the plants have been watered.
            // During each loop, water one plant.
            do
            {
            		water_a_plant_while_walking_away_from_the_faucet();

                if (!trip_done)
                {
                		walk_away_from_the_faucet_to_the_next_unwatered_plant();
                }
            } while (!trip_done);

            walk_back_to_the_faucet_with_an_empty_or_partially_filled_can();
        }

        cout << endl;
        cout << "Plan Near: Total steps = " << cum_steps
             << ", total step-units = " << cum_step_units << endl;
}

void plan_far()
{
    	    plan_title("Far", plant_count);
    	    reset();
        while (unwatered_count > 0)
        {
            // Fill the can.
            water_amount = UNITS_PER_CAN;

            walk_to_the_farthest_unwatered_plant_with_a_full_can();

            // Water plants while walking back towards the faucet.
            // Loop once per plant until either the can is empty
            // or all the plants have been watered.
            // During each loop, water one plant.
            do
            {
            		water_a_plant_while_walking_back_towards_the_faucet();

                if (!trip_done)
                {
                		walk_towards_the_faucet_to_the_next_unwatered_plant();
                }
            } while (!trip_done);

            walk_back_to_the_faucet_with_an_empty_or_partially_filled_can();
        }

        cout << endl;
        cout << "Plan Far: Total steps = " << cum_steps
             << ", total step-units = " << cum_step_units << endl;
}
