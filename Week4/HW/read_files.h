#pragma once

#include <bits/stdc++.h>
using namespace std;

struct Page {
    long long id;
    string title;
    double pagerank;
    bool visited = false;
};

void read_pages(const string& filename);

extern map<long long, set<long long>> id_to_ids;
// extern map<string, long long> title_to_id; 消す

void read_links(const string& filename);
void read_files();
