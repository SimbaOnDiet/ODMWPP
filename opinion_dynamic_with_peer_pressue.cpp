// opiniondynamicwithpeerpressue.cpp : 带有同侪压力的意见动力学模型模拟
//

#include "stdafx.h"		/* VS 2017 */

#include <iostream>		/* cin, cout, endl */
#include <fstream>		/* ofstream, close */
#include <stdio.h>		/* NULL */
#include <math.h>		/* sin	*/
#include <time.h>		/* time */
#include <stdlib.h>		/* rand, srand  */
#include <cstdlib>		/* RAND_MAX */
#include <vector>
#include <random>
#include <chrono>

using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::default_random_engine;
using std::normal_distribution;
using std::ofstream;


#define NODES 1024		//节点数
#define GROUPS 8		//群体数
#define BEFRIEND 0.5	//交朋友的概率
#define PRE_ZEAL 0.05	//狂热者比例
#define ROUND 1024		//模拟轮次
#define ACTIVE_OVER 2	//与反馈有关的常数，反应积极反馈的作用强烈程度
#define FRANK_THERHOLD 90	//Deprecated
#define NEW_FRIEND_PER_ROUND 2	//每一轮尝试交的新朋友个数
#define DISCONNECT_VALUE -60.0	//当一段友谊的优先度低于这个数的时候将会以断开告终
#define TRACK_NUM 2		//Deprecated

typedef struct _decision_para {
	float diff_mainstream;	//个体的公开意见中参考自己真实想法的比例
	float val_closeness;	//个人的友谊优先级当中亲密度的权重
	float courage_challenage;	//Deprecated
	float befriend;	//个人交朋友的意愿
	float active_respond;  //衡量一个人倾向于发表积极看法的程度
	float netural_respond; //衡量一个人倾向于发表中立看法的程度
} decision_para;	//与个人决策有关的参数

typedef struct _agent {
	typedef struct _relation {
		_agent* afriend;	//指向朋友的指针
		float closeness;	//亲密度
		float reliableness;	//可靠度
		//bool frank;
		float private_share_opinion;	//朋友私下向他分享的观点
		float priority;	//优先度
		_relation* counter;		//指向朋友对这段友谊的看法的指针
		int counter_part;	//Deprecated
	} relation;

	long int index;	//序号
	vector<relation> friends; //友谊
	vector<int> groups;	//群组

	float public_opinion;	//公开发表的意见
	float private_opinion;	//个人的真实想法

	decision_para para;		//决策产生参数
	bool zeal;	//是否狂热者
} agent;

//随机数产生器
default_random_engine generator;
normal_distribution<float> groups_dist(2.0, 8.0);	//!ADJUSTABLE PARA!
normal_distribution<float> public_opinion_dist(50.0, 15.0); //!ADJUSTABLE PARA!
normal_distribution<float> private_opinion_dist(50.0, 5.0); //!ADJUSTABLE PARA!
normal_distribution<float> diff_dist(50.0, 8.0);	//!ADJUSTABLE PARA!
normal_distribution<float> val_closeness_dist(50.0, 8.0);	//!ADJUSTABLE PARA!
normal_distribution<float> active_dist(50.0, 8.0);	//!ADJUSTABLE PARA!
normal_distribution<float> befriend_dist(60.0, 8.0);	//!ADJUSTABLE PARA!
normal_distribution<float> netural_dist(80.0, 8.0);	//!ADJUSTABLE PARA!
normal_distribution<float> courage_dist(30.0, 8.0);	//!ADJUSTABLE PARA!
normal_distribution<float> close_dist(50.0, 8.0);	//!ADJUSTABLE PARA!
normal_distribution<float> active_respond(80.0, 8.0);	//!ADJUSTABLE PARA!
normal_distribution<float> express_view_dist(8.0, 4.0);	//!ADJUSTABLE PARA!

unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937 mtgenerator(seed);
const unsigned _mtmaxrand = generator.max();

//网络数据存储
agent network[NODES];
vector<int> agent_tracked;

//产生网络
void construct_network(void);
//检查你的朋友的朋友列表中有没有你
void construct_both_friend(void);
//对朋友按优先度排列
void sort_friends(void);
//更新网络，包括分享观点等操作
void update_network(void);
enum attitude{possitive, neutrual, negative};
typedef struct _respond {
	attitude feedback;
	float extend;
} respond;
respond feedback(agent* target, agent::relation* relation, float share_value);
void friend_priority_update(agent* person);
void friendship_update(agent::relation* relation, respond criterion);
void making_newfriend(agent* person);
void bye2unreliable(agent* person);
void sharing_opinion(agent* person);
void opinion_update(agent* self,agent::relation* relation, respond criterion);
void pub_opinion_update(agent* self);
void write_down(void);
void tracking(void);
void setup_track(int num);
void reconstruct_relation(void);


ofstream public_op("public.txt");
ofstream private_op("private.txt");
ofstream tracking_file("tracking.txt");


int main()
{
	srand((unsigned)time(NULL));
	construct_network();
	//setup_track(TRACK_NUM);
	write_down();
	for (int round = 0; round < ROUND; round++) {
		update_network();
		write_down();
	}

	public_op.close();
	private_op.close();
    return 0;
}

void construct_network(void) {
	int tar_groups, cur_groups;
	int target;
	bool found;
	for (int i = 0; i < NODES; i++) {
		network[i].index = i;
#ifdef DEBUG
		tar_groups = 1;
#else
		do {
			tar_groups = (int)groups_dist(generator);
		} while (tar_groups<0 || tar_groups>GROUPS);
#endif
		cur_groups = 0;

		while (cur_groups < tar_groups) {
			target = mtgenerator() % GROUPS;
			found = false;
			for (int k = 0; k < (int)network[i].groups.size(); k++) {
				if (network[i].groups[k] == target) {
					found = true;
					break;
				}
			}
			if (!found) {
				network[i].groups.push_back(target);
				cur_groups++;
			}
		}

		//Initializing personal parameters
		do {
			network[i].public_opinion = public_opinion_dist(generator);
		} while (network[i].public_opinion < 0 || network[i].public_opinion>100);

		do {
			network[i].private_opinion = private_opinion_dist(generator);
		} while (network[i].private_opinion < 0 || network[i].private_opinion>100);
		if (mtgenerator() % 2 == 0) {
			network[i].private_opinion = -network[i].private_opinion;
		}
		

		do {
			network[i].para.netural_respond = netural_dist(generator);
		} while (network[i].para.netural_respond < 0 || network[i].para.netural_respond>100);

		do {
			network[i].para.active_respond = active_dist(generator);
		} while (network[i].para.active_respond < 0 || network[i].para.active_respond>100);
		network[i].para.active_respond *= network[i].para.netural_respond / 100.0F;

		do {
			network[i].para.befriend = befriend_dist(generator);
		} while (network[i].para.befriend < 0 || network[i].para.befriend>100);

		do {
			network[i].para.courage_challenage = courage_dist(generator);
		} while (network[i].para.courage_challenage < 0 || network[i].para.courage_challenage>100);

		do {
			network[i].para.diff_mainstream = diff_dist(generator);
		} while (network[i].para.diff_mainstream < 0 || network[i].para.diff_mainstream>100);

		do {
			network[i].para.val_closeness = close_dist(generator);
		} while (network[i].para.val_closeness < 0 || network[i].para.val_closeness>100);

		float attempt = (float)mtgenerator() / (float)_mtmaxrand;
		if (attempt < PRE_ZEAL) {
			network[i].zeal = true;
			if (network[i].private_opinion > 0) {
				network[i].public_opinion = 100;
				network[i].private_opinion = 100;
			}
			else {
				//network[i].public_opinion = -100;
				network[i].private_opinion = -100;
			}
		}
		else {
			network[i].zeal = false;
		}

		//Assign friends to newly added nodes according to groups
		agent::relation newrelationship;
#pragma omp parallel for
		for (int j = 0; j < i; j++) {
			int count_overlap = 0;
			for (int k = 0; k < (int)network[i].groups.size(); k++) {
				target = network[i].groups[k];
				for (int l = 0; l < (int)network[j].groups.size(); l++) {
					if (target == network[j].groups[l]) {
						count_overlap++;
						break;
					}
				}
			}
			float proba_befriend = BEFRIEND*count_overlap;
			float attempting = (long double)mtgenerator() / (long double)_mtmaxrand;

			if (attempting < proba_befriend) {
				newrelationship.afriend = &(network[j]);
				newrelationship.closeness = attempting / (1.0 - proba_befriend);
				//newrelationship.frank = false;
				newrelationship.private_share_opinion = network[i].public_opinion;
				newrelationship.reliableness = 0;

				newrelationship.priority = newrelationship.reliableness * 100.0 + newrelationship.closeness*network[i].para.val_closeness;

				(network[i].friends).push_back(newrelationship);
			}
		}

	}

	construct_both_friend();
	reconstruct_relation();
	sort_friends();
}

void construct_both_friend(void) {
	//If A is friend to B, B is friend to A
	for (int i = 0; i < NODES; i++) {
		int num_friends = network[i].friends.size();
		cout << "test!" << endl;
//#pragma omp parallel for
		for (int j = 0; j < num_friends; j++) {
			agent *ufriend = network[i].friends[j].afriend;
			int ufriend_numfriend = ufriend->friends.size();
			bool bothfriend = false;
			int matched;
			for (int k = 0; k < ufriend_numfriend; k++) {
				if ( &(network[i])== ufriend->friends[k].afriend ) {
					bothfriend = true;
					matched = k;
					break;
				}
			}
			agent::relation newrelationship;
			if (!bothfriend) {
				newrelationship = network[i].friends[j];
				newrelationship.afriend = &(network[i]);
				//newrelationship.counter = &(network[i].friends[j]);
				newrelationship.counter_part = j;
				ufriend->friends.push_back(newrelationship);
				//network[i].friends[j].counter = &(ufriend->friends[ufriend->friends.size() - 1]);
				network[i].friends[j].counter_part = ufriend->friends.size() - 1;
			}
			else {
				ufriend->friends[matched] = network[i].friends[j];
				ufriend->friends[matched].afriend = &(network[i]);
				//ufriend->friends[matched].counter = &(network[i].friends[j]);
				ufriend->friends[matched].counter_part = j;
				//network[i].friends[j].counter = &(ufriend->friends[matched]);
				network[i].friends[j].counter_part = matched;
			}
		}
	}
}

void sort_friends(void) {
	//Sorting the friends based on closeness and reliableness
	//Selecting Sorting
#pragma omp parallel for   //Different people do it at the same time
	for (int i = 0; i < NODES; i++) {
		int num_friends = network[i].friends.size();
		for (int j = 0; j < num_friends; j++) {
			int selected=j;
			float cur_moving = cur_moving = network[i].friends[j].priority;
			for (int k = j+1; k < num_friends; k++) {
				/*
				if (network[i].friends[k].frank == true) {
					selected = k;
					break;
				}
				*/
				//?
				if (cur_moving < network[i].friends[k].priority) {
					cur_moving = network[i].friends[k].priority;
					selected = k;
				}
			}

			//swaping
			agent::relation interswap;
			interswap = network[i].friends[selected];
			network[i].friends[selected] = network[i].friends[j];
			network[i].friends[j] = interswap;

			network[i].friends[selected].counter->counter = &(network[i].friends[selected]);
			//network[i].friends[selected].afriend->friends[network[i].friends[selected].counter_part].counter_part = selected;
			network[i].friends[j].counter->counter = &(network[i].friends[j]);
			//network[i].friends[j].afriend->friends[network[i].friends[j].counter_part].counter_part = j;
		}
	}
}

//Unfinished
/*
void update_network(void) {
	//Update public opinion and private opinion seperatively
	for (int cur_man = 0; cur_man < NODES; cur_man++) {
		int num_friends = network[cur_man].friends.size();
		float sum_public_opinion = 0;
#pragma omp parallel for  
		for (int cur_fri = 0; cur_fri < num_friends; cur_fri++) {
			sum_public_opinion += network[cur_man].friends[cur_fri].afriend->public_opinion;
		}
		float tmp_opinion = sum_public_opinion / num_friends;
		network[cur_man].public_opinion = (float)(network[cur_man].para.diff_mainstream*network[cur_man].private_opinion + (100 - network[cur_man].para.diff_mainstream)*network[cur_man].public_opinion) / 100.0;

		//Update private opinion based on feedback
#pragma omp parallel for  
		for (int cur_fri = 0; cur_fri < num_friends; cur_fri++) {
			if (network[cur_man].friends[cur_fri].afriend->zeal) {
				if (network[cur_man].friends[cur_fri].afriend->private_opinion > 0) {

				}
			}
			else if (network[cur_man].friends[cur_fri].frank) {
				// Feedback procedure 
				if (abs(network[cur_man].private_opinion - network[cur_man].friends[cur_fri].afriend->private_opinion) < network[cur_man].friends[cur_fri].afriend->para.active_respond) {
					float weight;
					do {
						weight = active_respond(generator);
					} while (weight > 100 || weight < 0);
					network[cur_man].friends[cur_fri].afriend->private_opinion = (100.0*network[cur_man].friends[cur_fri].afriend->private_opinion + ACTIVE_OVER*weight*(network[cur_man].friends[cur_fri].counter->priority / 100.0)*network[cur_man].private_opinion) / (100 + weight);
					do {
						weight = active_respond(generator);
					} while (weight > 100 || weight < 0);
					network[cur_man].private_opinion = (100.0*network[cur_man].private_opinion + ACTIVE_OVER*weight*(network[cur_man].friends[cur_fri].priority / 100.0)*network[cur_man].friends[cur_fri].afriend->private_opinion) / (100 + weight);


					do {
						weight = active_respond(generator);
					} while (weight > 100 || weight < 0);
					network[cur_man].friends[cur_fri].reliableness += ACTIVE_OVER*(1 - network[cur_man].friends[cur_fri].reliableness)*active_dist(generator) / 100;
					do {
						weight = active_respond(generator);
					} while (weight > 100 || weight < 0);
					network[cur_man].friends[cur_fri].counter->reliableness += ACTIVE_OVER*(1 - network[cur_man].friends[cur_fri].counter->reliableness)*active_dist(generator) / 100;
				}
				else if (abs(network[cur_man].private_opinion - network[cur_man].friends[cur_fri].afriend->private_opinion) < network[cur_man].friends[cur_fri].afriend->para.netural_respond) {
					float weight;
					do {
						weight = active_respond(generator);
					} while (weight > 100 || weight < 0);
					network[cur_man].friends[cur_fri].afriend->private_opinion = (100.0*network[cur_man].friends[cur_fri].afriend->private_opinion + weight*(network[cur_man].friends[cur_fri].counter->priority / 100.0)*network[cur_man].private_opinion) / (100 + weight);
					do {
						weight = active_respond(generator);
					} while (weight > 100 || weight < 0);
					network[cur_man].private_opinion = (100.0*network[cur_man].private_opinion + weight*(network[cur_man].friends[cur_fri].priority / 100.0)*network[cur_man].friends[cur_fri].afriend->private_opinion) / (100 + weight);

					do {
						weight = active_respond(generator);
					} while (weight > 100 || weight < 0);
					network[cur_man].friends[cur_fri].reliableness += (1 - network[cur_man].friends[cur_fri].reliableness)*active_dist(generator) / 100;
					do {
						weight = active_respond(generator);
					} while (weight > 100 || weight < 0);
					network[cur_man].friends[cur_fri].counter->reliableness += (1 - network[cur_man].friends[cur_fri].counter->reliableness)*active_dist(generator) / 100;

				}
			}
		}
	}
}
*/

respond feedback(agent* target,agent::relation* relation, float share_value) {
	respond result;
	respond feeling;
	relation->private_share_opinion = share_value;
	if (target->zeal) {
		if (target->private_opinion > 0) {
			if (share_value < 0) {
				result.feedback = negative;
				result.extend = 100;
			}
			if (share_value > 0) {
				result.feedback = possitive;
				result.extend = 100-sqrt(100-share_value);
			}
		}
		else{
			if (share_value < 0) {
				result.feedback = possitive;
				result.extend = 100 - sqrt(100 - fabs(share_value));
			}
			if (share_value > 0) {
				result.feedback = neutrual;
				result.extend = 100;
			}
		}
		feeling = result;
	}
	else {
		if (fabs(target->private_opinion - share_value) < target->para.active_respond) {
			result.feedback = possitive;
			result.extend = active_respond(generator);
			feeling = result;
		}
		else if (fabs(target->private_opinion - share_value) < target->para.netural_respond) {
			result.feedback = neutrual;
			result.extend = active_respond(generator);
			feeling = result;
		}
		else {
			if (relation->reliableness < 0 && target->private_opinion<0 && share_value>0) {
				result.feedback = neutrual;
				result.extend = 100;
				feeling.feedback = negative;
				feeling.extend = active_respond(generator);

			}
			else if (target->private_opinion<0 && share_value>0){
				result.feedback = negative;
				float weight= active_respond(generator);
				result.extend = weight*relation->reliableness / 100.0;
				feeling.feedback = negative;
				feeling.extend = weight;
			}
			else {
				result.feedback = negative;
				result.extend = active_respond(generator);
				feeling = result;
			}
		}
		opinion_update(target,relation, feeling);	//Update the receivor agent
		opinion_update(relation->afriend, relation->counter, result);	//Update the source agent

		friendship_update(relation, feeling);
		friendship_update(relation->counter, result);
	}

	return result;
}

void friendship_update(agent::relation* relation, respond criterion) {
	float weight;
	switch (criterion.feedback) {
	case possitive: {
		do {
			weight = active_respond(generator);
		} while (weight > 100 || weight < 0);
		relation->reliableness += (100 - relation->reliableness)*(criterion.extend / 100.0)*(ACTIVE_OVER*active_respond(generator)/ (ACTIVE_OVER*100.0));
		break;
	}
	case neutrual: {
		if (relation->private_share_opinion < 0) {
			do {
				weight = active_respond(generator);
			} while (weight > 100 || weight < 0);
			relation->reliableness += (100 - relation->reliableness)*(criterion.extend / 100.0)*(active_respond(generator) / (ACTIVE_OVER*100.0));
		}
		break;
	}
	case negative: {
		do {
			weight = active_respond(generator);
		} while (weight > 100 || weight < 0);
		relation->reliableness -= abs(relation->reliableness)*(criterion.extend / 100.0)*(active_respond(generator) / (ACTIVE_OVER*100.0));
		break;
	}
	}
}

void opinion_update(agent* self, agent::relation* relation, respond criterion) {
	if (self->zeal) {
		/*Do Nothing*/
	}
	else {
		switch (criterion.feedback) {
		case possitive: {
			self->private_opinion = (self->private_opinion * 100.0 + ACTIVE_OVER*relation->private_share_opinion*criterion.extend) / (100 + ACTIVE_OVER * 100);
			break;
		}
		case neutrual: {
			self->private_opinion = (self->private_opinion * 100.0 + relation->private_share_opinion*criterion.extend) / (100 + ACTIVE_OVER * 100);
			break;
		}
		case negative: {
			/* Do nothing */
			break;
		}
		}
	}
}

void friend_priority_update(agent* person) {
	//Recalculating the priority
	int num_friends = person->friends.size();
#pragma omp parallel for
	for (int i = 0; i < num_friends; i++) {
		person->friends[i].priority = (person->friends[i].closeness*person->para.val_closeness / 100.0 + person->friends[i].reliableness) / 200.0;
	}
}

void making_newfriend(agent* person) {
	int num_friends = person->friends.size();
	int newly_connected = 0;
	int total = 3;
	int candidate_A=0, candidate_B=3;
	if (num_friends <= 2) {
		return;
	}
	while (newly_connected<NEW_FRIEND_PER_ROUND && total<num_friends - 1) {
		if (++candidate_A > --candidate_B) {
			total++;
			candidate_A = 0;
			candidate_B = total;
			continue;
		}
		float attempt = (float)mtgenerator() / (float)_mtmaxrand;
		float criterion = BEFRIEND*(person->friends[candidate_A].reliableness / 100.0)*(person->friends[candidate_B].reliableness / 100.0);

		if (attempt < criterion) {
			newly_connected++;
			/* let's be freinds */
			int num_friendsfriend = person->friends[candidate_A].afriend->friends.size();
			bool alreadyfriend = false;
#pragma omp parallel for shared(alreadyfriend)
			for (int i = 0; i < num_friendsfriend; i++) {
				if (person->friends[candidate_A].afriend->friends[i].afriend == person->friends[candidate_B].afriend) {
					alreadyfriend = true;
				}
			}
				if (!alreadyfriend) {
					agent::relation newrelationshipAB;
					agent::relation newrelationshipBA;

					newrelationshipAB.afriend = person->friends[candidate_B].afriend;
					newrelationshipAB.closeness = (person->friends[candidate_A].counter->closeness / 100.0)*(person->friends[candidate_B].closeness / 100.0)*100.0;
					newrelationshipAB.reliableness = (person->friends[candidate_A].counter->reliableness / 100.0)*(person->friends[candidate_B].reliableness / 100.0)*100.0;
					/* Priority will be update afterward globally */

					newrelationshipAB.private_share_opinion = (person->friends[candidate_A].private_share_opinion + person->friends[candidate_B].private_share_opinion) / 2.0;
					newrelationshipBA.private_share_opinion = newrelationshipAB.private_share_opinion;

					newrelationshipBA.afriend = person->friends[candidate_A].afriend;
					newrelationshipBA.closeness= (person->friends[candidate_B].counter->closeness / 100.0)*(person->friends[candidate_A].closeness / 100.0)*100.0;
					newrelationshipBA.reliableness= (person->friends[candidate_B].counter->reliableness / 100.0)*(person->friends[candidate_A].reliableness / 100.0)*100.0;

					person->friends[candidate_A].afriend->friends.push_back(newrelationshipAB);
					person->friends[candidate_B].afriend->friends.push_back(newrelationshipBA);

					person->friends[candidate_A].afriend->friends[person->friends[candidate_A].afriend->friends.size() - 1].counter = &(person->friends[candidate_B].afriend->friends[person->friends[candidate_B].afriend->friends.size() - 1]);
					person->friends[candidate_B].afriend->friends[person->friends[candidate_B].afriend->friends.size() - 1].counter = &(person->friends[candidate_A].afriend->friends[person->friends[candidate_A].afriend->friends.size() - 1]);
				}
			}
		}
	}

//Unfinished
void sharing_opinion(agent* person) {
	int num_friends = person->friends.size();
	float warning = (person->private_opinion - person->public_opinion) / 200.0;

	if ((warning = (person->private_opinion - person->public_opinion) / 200.0) < 0) {
		warning = -warning;
	}
	else {
		warning = 0;
	}

	int num_of_com ;
	do {
		num_of_com = (int)express_view_dist(generator);
	} while (num_of_com < 0 || num_of_com>num_friends);
	int prior = num_of_com*warning;
	int random_tell = num_of_com - prior;
	if (!person->zeal) {
#pragma omp parallel for
		for (int i = 0; i < prior; i++) {
			feedback(person->friends[i].afriend, person->friends[i].counter, ((person->friends[i].reliableness / 100.0)*person->private_opinion + person->public_opinion) / ((person->friends[i].reliableness / 100.0) + 1));
		}
		int random_chosen = 0;
		for (int i = prior; i < num_friends; i++) {
			float attempt = (float)mtgenerator() / (float)_mtmaxrand;
			if (attempt < (person->friends[i].priority / 100.0)) {
				feedback(person->friends[i].afriend, person->friends[i].counter, ((person->friends[i].reliableness / 100.0)*person->private_opinion + person->public_opinion) / ((person->friends[i].reliableness / 100.0) + 1));
			}
			if (++random_chosen > random_tell) {
				break;
			}
		}
	}
	else {
		for (int i = 0; i < num_friends; i++) {
			feedback(person->friends[i].afriend, person->friends[i].counter, person->private_opinion);
		}
	}
}

void bye2unreliable(agent* person) {
	if (person->friends.size() == 0) {
		return;
	}
	while (person->friends[person->friends.size() - 1].priority < -60.0) {
		for (int i =(int) person->friends[person->friends.size() - 1].afriend->friends.size() - 1; i >= 0; i--) {
			if (person->friends[person->friends.size() - 1].afriend->friends[i].afriend == person) {
				vector<agent::relation>::iterator it;
				it = person->friends[person->friends.size() - 1].afriend->friends.begin() + i;
				person->friends[person->friends.size() - 1].afriend->friends.erase(it);
				break;
			}
		}
		person->friends.pop_back();
	}
}

void pub_opinion_update(agent* self) {
	int num_friends = self->friends.size();
	if (!self->zeal) {
		int num_friends = self->friends.size();
		float sum_public_opinion = 0;
#pragma omp parallel for  
		for (int cur_fri = 0; cur_fri < num_friends; cur_fri++) {
			sum_public_opinion += self->friends[cur_fri].afriend->public_opinion;
		}
		float tmp_opinion = sum_public_opinion / num_friends;
		self->public_opinion = (float)(self->para.diff_mainstream*self->private_opinion + (100 - self->para.diff_mainstream)*self->public_opinion) / 100.0;
	}
	else {
		/* Do Nothing */
	}
}

void write_down(void) {
	for (int i = 0; i < NODES; i++) {
		public_op << network[i].public_opinion << "\t";
	}
	public_op << endl;
	for (int i = 0; i < NODES; i++) {
		private_op << network[i].private_opinion << "\t";
	}
	private_op << endl;
}

void update_network(void) {
	for (int cur_man = 0; cur_man < NODES; cur_man++) {
		pub_opinion_update(&(network[cur_man]));
	}
	for (int cur_man = 0; cur_man < NODES; cur_man++) {
		sharing_opinion(&(network[cur_man]));
	}
	for (int cur_man = 0; cur_man < NODES; cur_man++) {
		bye2unreliable(&(network[cur_man]));
	}
	for (int cur_man = 0; cur_man < NODES; cur_man++) {
		making_newfriend(&(network[cur_man]));
	}

	reconstruct_relation();
	sort_friends();
}

void tracking(void) {
	int track_num = agent_tracked.size();
	for (int i = 0; i < track_num; i++) {
		tracking_file << network[agent_tracked[i]].public_opinion << "\t" << network[agent_tracked[i]].private_opinion << "\t";
	}
	tracking_file << endl;
}

void setup_track(int num) {
	int count = 0;
	int selecting;
	while (count < num) {
		selecting = mtgenerator() % NODES;
		bool found = false;
		for (int j = 0; j < (int)agent_tracked.size(); j++) {
			if (selecting == agent_tracked[j]) {
				found = true;
				break;
			}
		}
		if (!found) {
			agent_tracked.push_back(selecting);
			count++;
		}
	}
}

void reconstruct_relation(void) {
	for (int i = 0; i < NODES; i++) {
		int num_friends = network[i].friends.size();
		for (int j = 0; j < num_friends; j++) {
			if (network[i].friends[j].afriend->index < i) {
				continue;	//It must has been constructed!
			}
			/*
			if (network[i].friends[j].counter != NULL) {
				if (network[i].friends[j].counter->counter != NULL) {
					continue;	//No problem with the counter para
				}
			}
			*/
			
			bool failtofound = true;
			agent* ufriend = network[i].friends[j].afriend;
			int num_ufriends = ufriend->friends.size();
			int matched;
			for (int k = 0; k < num_ufriends; k++) {
				if (&(network[i]) == ufriend->friends[k].afriend) {
					matched = k;
					failtofound = false;
				}
			}

			if (!failtofound) {
				ufriend->friends[matched].counter = &(network[i].friends[j]);
				network[i].friends[j].counter = &(ufriend->friends[matched]);
			}
			else {
				ofstream error("error.txt");
				error << "Failed to find corresponding friend!" << endl;
				error.close();
				exit(EXIT_FAILURE);
			}
		}
	}
}