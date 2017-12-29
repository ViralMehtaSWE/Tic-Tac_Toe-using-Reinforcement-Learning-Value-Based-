#include <bits/stdc++.h>
#define mod 1000000007
#define INF 1000000000000000007LL
#define SZ 5000000
#define WIN_REWARD 1
#define LOSS_REWARD -1
#define DRAW_REWARD -0.5
#define PROGRESS_REWARD 0
#define GAMMA 1
#define ALPHA 0.1
#define NUM_STATES 20000
#define MAX_NUM_ACTIONS 10
#define ull unsigned long long
#define ll long long int
#define ld long double
#define fi first
#define se second
#define pb push_back
#define pf push_front
#define ppb pop_back
#define ppf pop_front
#define all(v) ((v).begin()), ((v).end())
#define ios ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);
#define db(...) ZZ(#__VA_ARGS__, __VA_ARGS__)
#define dbvv(v) cout << "Printing "#v << " --> \n"; for(ll i=0;i<v.size();i++) {for(ll j=0;j<v[i].size();j++) cout << v[i][j] << " "; cout << "\n";}
#define dbv(st) cout << "Printing "#st << " --> \n"; for(auto i=st.begin();i!=st.end();i++) cout << *i << " "; cout << "\n";
#define dbvp(st) cout << "Printing "#st << " --> \n"; for(auto i=st.begin();i!=st.end();i++) cout << i->first << " " << i->second << "\n"; cout << "\n";
#define dbmp(mp) cout << "Printing "#mp << " --> \n"; for(auto i=mp.begin();i!=mp.end();i++) cout << #mp"[" << i->first << "]"<< " = " << i->second << "\n";
#define dbar(ar,st,en) cout << "Printing "#ar << " --> \n"; for(auto it=st;it!=en;it++) cout << *it << " "; cout << "\n";
#define dbmar(ar,rowz,colz) cout << "Printing "#ar << " --> \n"; for(auto i=0;i<rowz;i++) {for(auto j=0;j<colz;j++) cout << ar[i][j] << " "; cout << "\n";}
template <typename Arg1>void ZZ(const char* name, Arg1&& arg1){std::cout << name << " = " << arg1 << std::endl;}
template <typename Arg1, typename... Args>void ZZ(const char* names, Arg1&& arg1, Args&&... args)
{
    const char* comma = strchr(names + 1, ',');
    std::cout.write(names, comma - names) << " = " << arg1;
    ZZ(comma, args...);
}
using namespace std;

ll to_ll(string s)
{
	ll i,ret=0,p=1;
	for(i=(ll)s.length()-1;i>=0;i--) ret+=(s[i]-'0')*p, p*=10LL;
	return ret;
}
ll gcd(ll x,ll y)
{
	if(y==0) return x;
	return gcd(y,x%y);
}
ll pwr(ll base,ll expo,ll m)
{
	if(base==0) return 0LL;
	if(expo==0) return (1LL%m);
	if((expo&1)==0) {ll temp=pwr(base,expo>>1,m); return (temp*temp)%m;}
	return ((base%m)*pwr(base,expo-1,m))%m;
}
/*-----------------------------------------------------------------------------------------------------------------------------------------
##########################
## Author: Viral Mehta  ##
## College: BITS Pilani ##
##########################
-----------------------------------------------------------------------------------------------------------------------------------------*/
bool is_game_over(ll state);
vector<ll> get_board_list(ll x);
bool is_Xs_turn(ll state);

ld Q_table[NUM_STATES][MAX_NUM_ACTIONS];
vector<ll> pwrs3 = {1, 3, 9, 27, 81, 243, 729, 2187, 6561};
void initialize_Q_table()
{
	ll i,j;
	for(i=0;i<NUM_STATES;i++)
		for(j=0;j<MAX_NUM_ACTIONS;j++)
			Q_table[i][j] = 0*rand()/(ld)(RAND_MAX);
}
void find_mx_mn()
{
	ld mx = -INF;
	ld mn = INF;
	ll i, j, mx_state, mx_action, mn_state, mn_action;
    for(i=0;i<NUM_STATES;i++)
    {
        for(j=0;j<MAX_NUM_ACTIONS;j++)
        {
			mx=max(mx,Q_table[i][j]);
			mn=min(mn,Q_table[i][j]);
			if(mx == Q_table[i][j])
			{
				mx_state = i;
				mx_action = j;
			}
			if(mn == Q_table[i][j])
			{
				mn_state = i;
				mn_action = j;
			}
		}
	}
	db(mx,mx_state,mx_action,mn,mn_state,mn_action);
	cout.flush();
}
vector< pair<ll,ll> > get_next_states(ll state)
{
	vector<ll> board;
	vector< pair<ll,ll> > next_states;
	if(is_game_over(state)) return next_states;
	ll val=state;
	ll X=0,Y=0;
	while(val>0||board.size()<9)
	{
		ll rem=val%3;
		board.pb(rem);
		val/=3;
		if(rem==2) X++;
		else if(rem==1) Y++;
	}
	if(!(X>=Y))
	{
		db(state,X,Y);
		dbv(board);
	}
	assert(X>=Y); 
	assert(board.size()==9);
	for(ll i=0;i<9;i++)
	{
		if(board[i]==0)
		{
			if(X>Y) next_states.pb({i,state+1*pwrs3[i]});
			else next_states.pb({i,state+2*pwrs3[i]});
		}
	}
	return next_states;
}

pair<ll,ll> carry_out_best_action(ll state, bool test=false)
{
	vector< pair<ll,ll> > options = get_next_states(state);
	ld mx=-INF;
	ll idx=-1;

	for(ll i=0;i<options.size();i++)
	{
		if(mx<Q_table[state][options[i].first])
		{
			mx = Q_table[state][options[i].first];
			idx = i;
		}
	}

	if(idx==-1) return {9,-1};

	if(!test)
	{
		ld val = rand()/(double)(RAND_MAX);
		if(val<=0.1) idx=rand()%options.size();
	}

	return options[idx];
}

pair<ll,ll> carry_out_worst_action(ll state, bool test=false)
{
	vector< pair<ll,ll> > options = get_next_states(state);
	ld mn=INF;
	ll idx=-1;

	for(ll i=0;i<options.size();i++)
	{
		if(mn>Q_table[state][options[i].first])
		{
			mn = Q_table[state][options[i].first];
			idx = i;
		}
	}

	if(idx==-1) return {9,-1};

	if(!test)
	{
		ld val = rand()/(double)(RAND_MAX);
		if(val<=0.1) idx=rand()%options.size();
	}

	return options[idx];
}

bool is_winner(vector<ll>& board, ll typ)
{
	if(board[0]==typ&&board[1]==typ&&board[2]==typ) return true;
	if(board[3]==typ&&board[4]==typ&&board[5]==typ) return true;
	if(board[6]==typ&&board[7]==typ&&board[8]==typ) return true;

	if(board[0]==typ&&board[3]==typ&&board[6]==typ) return true;
	if(board[1]==typ&&board[4]==typ&&board[7]==typ) return true;
	if(board[2]==typ&&board[5]==typ&&board[8]==typ) return true;

	if(board[0]==typ&&board[4]==typ&&board[8]==typ) return true;
	if(board[2]==typ&&board[4]==typ&&board[6]==typ) return true;

	return false;
}

bool is_draw(vector<ll>& board)
{
	ll cnt = count(all(board),0);
	if(cnt==0&&is_winner(board,1)==false&&is_winner(board,2)==false) return true;
	return false;
}

bool is_game_over(ll state)
{
	vector<ll> board = get_board_list(state);
	if(is_draw(board)||is_winner(board,1)||is_winner(board,2)) return true;
	return false;
}

bool is_Xs_turn(ll state)
{
	vector<ll> board = get_board_list(state);
	ll cnt1 = count(all(board), 1);
	ll cnt2 = count(all(board), 2);
	return(cnt1==cnt2);
}

ll get_reward(ll state)
{
	vector<ll> board;
	ll val=state;
	ll X=0,Y=0;
	while(val>0||board.size()<9)
	{
		ll rem=val%3;
		board.pb(rem);
		val/=3;
		if(rem==2) X++;
		else if(rem==1) Y++;
	}
	assert(X>=Y);
	assert(board.size()==9);
	if(X>Y)
	{
		if(is_winner(board,2)) return WIN_REWARD;
		else if(is_draw(board)) return DRAW_REWARD;
		return PROGRESS_REWARD;
	}
	else
	{
		if(is_winner(board,1)) return LOSS_REWARD;
		return PROGRESS_REWARD;
	}
}
vector<ll> get_board_list(ll x)
{
	vector<ll> ret;
	while(ret.size()<9)
	{
		ret.pb(x%3);
		x=x/3;
	}
	return ret;
}
void print_board(ll x)
{
	vector<ll> v = get_board_list(x);
	cout << "Printing Q-values: \n";
	cout << setprecision(8) << fixed << Q_table[x][0] << " | " << Q_table[x][1] << " | " << Q_table[x][2] << "\n";
	cout << setprecision(8) << fixed << Q_table[x][3] << " | " << Q_table[x][4] << " | " << Q_table[x][5] << "\n";
	cout << setprecision(8) << fixed << Q_table[x][6] << " | " << Q_table[x][7] << " | " << Q_table[x][8] << "\n";
	cout << "--------------------------------\n";
	cout << v[0] << " | " << v[1] << " | " << v[2] << "\n";
	cout << v[3] << " | " << v[4] << " | " << v[5] << "\n";
	cout << v[6] << " | " << v[7] << " | " << v[8] << "\n";
	cout << "################################\n";
	cout.flush();
}
ll modify_board(ll state, ll pos)
{
	return state + 1*pwrs3[pos];
}
void train(ll num_iters)
{
	initialize_Q_table();
	ll state=0;
	while(num_iters--)
	{
		if(is_Xs_turn(state))
		{
			pair<ll,ll> current_action_and_next_state = carry_out_best_action(state);
			ll current_action = current_action_and_next_state.first;
			assert(current_action!=9);
			ll next_state = current_action_and_next_state.second;
			ll reward = get_reward(next_state);
			pair<ll,ll> best_action_from_next_state = carry_out_worst_action(next_state);
			ll best_action = best_action_from_next_state.first;
			Q_table[state][current_action] = Q_table[state][current_action] + ALPHA*(reward + GAMMA*Q_table[next_state][best_action] - Q_table[state][current_action]);
			/*if(state==866&&current_action==1) 
			{
				db(num_iters, state, current_action, reward, next_state, best_action, Q_table[next_state][best_action], Q_table[state][current_action]);
				cout.flush();
			}*/
			if(best_action != 9) state = next_state;
			else state = 0;
		}
		else
		{
			pair<ll,ll> current_action_and_next_state = carry_out_worst_action(state);
			ll current_action = current_action_and_next_state.first;
			assert(current_action!=9);
			ll next_state = current_action_and_next_state.second;
			ll reward = get_reward(next_state);
			pair<ll,ll> best_action_from_next_state = carry_out_best_action(next_state);
			ll best_action = best_action_from_next_state.first;
			//db(num_iters, state, current_action, reward, next_state, best_action);
			Q_table[state][current_action] = Q_table[state][current_action] + ALPHA*(reward + GAMMA*Q_table[next_state][best_action] - Q_table[state][current_action]);
			if(best_action != 9) state = next_state;
			else state = 0;
		}
	}
	find_mx_mn();
}
void play_game()
{
	ll pos, t=4, state=0;
	vector<ll> board;
	while(t--)
	{
		pair<ll,ll> next_info = carry_out_best_action(state, true);
		//db(state,next_info.first, next_info.second);
		state = next_info.second;
		print_board(state);
		board = get_board_list(state);
		if(is_winner(board, 2))
		{
			cout << "Sorry, You Lost!\n";
			cout.flush();
			return;
		}
		cout << "Enter your move: ";
		cout.flush();
		cin>>pos;
		if(pos==-1)
		{
			// For Debugging ///////////////////////////////////////////////////////////////
			while(1)
			{
				cout << "Enter Query State, Action: ";
				cout.flush();
				ll s,a;
				cin>>s>>a;
				cout << "Q_table[" << s << "][" << a << "] = " << Q_table[s][a] << "\n";
			}
			////////////////////////////////////////////////////////////////////////////////
		}
		state=modify_board(state,pos);
		board = get_board_list(state);
		if(is_winner(board, 1))
		{
			print_board(state);
			cout << "Congratulations, You Won!\n";
			cout.flush();
			return;
		}
	}
	cout << "Its a Draw!\n";
	cout.flush();
}
int main()
{
	ios;
	ll num_iters;
	cout << "Enter no. of training iterations: ";
	cout.flush();
	cin >> num_iters;
	cout << "Training......\n";
	cout.flush();
	train(num_iters);
	cout << "Training Completed!\n";
	cout.flush();
	while(1)
	{
		play_game();
	}
	return 0;
}
