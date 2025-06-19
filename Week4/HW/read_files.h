#pragma once

#include <bits/stdc++.h>
using namespace std;

struct Page {
    long long id;
    string title;
    double pagerank;
    bool visited = false;
};

map<string, long long> title_to_id;
map<long long, Page> id_to_page;
void read_pages(const string& filename);

map<long long, set<long long>> id_to_ids;
void read_links(const string& filename);

void read_files();
