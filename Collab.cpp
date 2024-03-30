#include <bits/stdc++.h>
#include <fstream>
using namespace std;

vector<pair<int, int>> edge_list;

void input(string fileName)
{
	ifstream inputFile(fileName);
	int N1, N2;
	inputFile >> N1;
	inputFile >> N2;

	int E;
	inputFile >> E;

	pair<int, int> p;
	for (int i = 0; i < E; i++)
	{
		inputFile >> p.first;
		inputFile >> p.second;

		edge_list.push_back(p);
	}
}

map<int, int> calculate_degree()
{
	map<int, int> degree;
	for (auto edge : edge_list)
	{
		degree[edge.first]++;
		degree[edge.second]++;
	}
	return degree;
}

vector<int> min_degree_nodes(map<int, int> &deg)
{
	vector<int> u;

	int min_deg = INT_MAX;

	for (auto pair : deg)
		if (pair.second < min_deg)
			min_deg = pair.second;

	cout << "MIN DEGREE: " << min_deg << endl;

	for (auto pair : deg)
		if (min_deg != INT_MAX and pair.second == min_deg)
			u.push_back(pair.first);

	return u;
}

map<int, int> find_adjacent_nodes_degrees(vector<int> &min_degree_nodes, map<int, int> &deg)
{
	set<int> adjacent_nodes;
	for (auto node : min_degree_nodes)
		for (auto edge : edge_list)
			if (edge.first == node)
				adjacent_nodes.insert(edge.second);
			else if (edge.second == node)
				adjacent_nodes.insert(edge.first);

	map<int, int> adj_nodes_degrees;
	for (auto node : adjacent_nodes)
		adj_nodes_degrees[node] = deg[node];

	return adj_nodes_degrees;
}

vector<int> result(vector<int> min_degree_nodes, map<int, int> deg)
{
	set<int> result;
	for (auto node : min_degree_nodes)
	{
		set<int> adj_nodes_to_given_node;
		for (auto edge : edge_list)
		{
			if (edge.first == node)
			{
				adj_nodes_to_given_node.insert(edge.second);
			}
			else if (edge.second == node)
			{
				adj_nodes_to_given_node.insert(edge.first);
			}
		}
		map<int, int> adj_nodes_degrees;
		for (auto node : adj_nodes_to_given_node)
		{
			adj_nodes_degrees[node] = deg[node];
		}

		int min_degree = INT_MAX;
		for (auto pair : adj_nodes_degrees)
		{
			if (pair.second < min_degree)
				min_degree = pair.second;
		}
		for (auto pair : adj_nodes_degrees)
		{
			if (pair.second == min_degree)
				result.insert(pair.first);
		}
	}
	vector<int> ans(result.begin(), result.end());
	return ans;
}

void modify_edge_list(vector<int> &m)
{

	vector<pair<int, int>> new_edge_list;
	for (auto edge : edge_list)
	{
		int first_node = edge.first;
		int second_node = edge.second;
		auto itr1 = find(m.begin(), m.end(), first_node);
		auto itr2 = find(m.begin(), m.end(), second_node);

		if ((itr1 == m.end()) && (itr2 == m.end()))
			new_edge_list.push_back(edge);
	}

	edge_list = new_edge_list;
}

vector<int> CSSA()
{
	vector<int> C;

	while (!edge_list.empty())
	{
		cout << "NO OF EDGES : " << edge_list.size() << endl;
		/*
			step - 1

			-> Calculate the deg (V) in G = (V, E)
			-> input : edge_list
			-> output : degrees in a map

		*/

		map<int, int> deg = calculate_degree();

		/*
			step - 2

			-> (i) Find out min (G) and u <- min(G).
			-> (ii) Find out Adj(u).
			-> input : degree map and edge_list
			-> output : min deg nodes and adjacent(min degree nodes)
		*/

		vector<int> u = min_degree_nodes(deg);
		cout << "min deg nodes size : " << u.size() << endl;
		map<int, int> adj_nodes_degrees = find_adjacent_nodes_degrees(u, deg);
		cout << "adj nodes list size : " << adj_nodes_degrees.size() << endl;
		/*
			step - 3

			-> (i) Find out min (adj(u)) and m <- min(adj(u)).
			-> (ii) V <- V - {m}.
			-> (iii) C <- C U {m}.
		*/

		vector<int> m = min_degree_nodes(adj_nodes_degrees);
		// vector<int> m = result(u,deg);
		modify_edge_list(m);
		C.insert(C.end(), m.begin(), m.end());
	}

	return C;
}

int main()
{
	string fileName;
	cin >> fileName;
	input(fileName);
	vector<int> C = CSSA();

	cout << C.size() << endl;
	for (auto element : C)
		cout << element << "\n";
	cout << endl;
}