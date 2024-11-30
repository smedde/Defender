#define map_size_rows 12
#define map_size_cols 24

#include <limits.h>
#include <float.h>
#include <iso646.h>

char map[map_size_rows][map_size_cols] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1},
	{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
	{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1},
	{1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
	{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
	{1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

/* description of graph node */
struct stop {
	short int col;
	short int row;
	/* array of indexes of routes from this stop to neighbours in array of all routes */
	//int* n;
	short int n[8];
	short int n_len;
	short int f, g, h;
	short int from;
};

int ind[map_size_rows][map_size_cols] = {
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
};

/* description of route between two nodes */
struct route {
	/* route has only one direction! */
	short int x; /* index of stop in array of all stops of src of this route */
	short int y; /* intex of stop in array of all stops od dst of this route */
	short int d;
};


int test() {
	int i, j, k, l, b, found;
	int p_len = 0;
	//int* path = NULL;
	short int path[1200];
	int c_len = 0;
	//int* closed = NULL;
	short int closed[220];
	int o_len = 1;
	//int* open = (int*)calloc(o_len, sizeof(int));
	short int open[220];
	float min, tempg;
	int s;
	int e;
	int current;
	int s_len = 0;
	//struct stop* stops = NULL;
	struct stop stops[(map_size_rows-4) * (map_size_cols-4)];
	int r_len = 0;
	//struct route* routes = NULL;
	struct route routes[850];
	
	//checkStack();

	int startX = 3;
	int startY = 5;
	
	int targetX = 7;
	int targetY = 9;


	for (i = 1; i < map_size_rows - 1; i++) {
		for (j = 1; j < map_size_cols - 1; j++) {
			if (!map[i][j]) {
				++s_len;
				//stops = (struct stop*)realloc(stops, s_len * sizeof(struct stop));
				//printf("stops: %i \n", s_len );
				int t = s_len - 1;
				stops[t].col = j;
				stops[t].row = i;
				stops[t].from = -1;
				stops[t].g = DBL_MAX;
				stops[t].n_len = 0;
				//stops[t].n = NULL;
				ind[i][j] = t;

				if ((i == startY ) && (j == startX)) {
					s = s_len-1;
				}
				if ((i == targetY) && (j == targetX-1)) {
					e = s_len;
				}
			}
		}
	}

	/* index of start stop, counts only active blocks*/
	//s = 0;
	/* index of finish stop */
	//e = 100; // s_len - 1;
	
	for (i = 0; i < s_len; i++) {
		stops[i].h = sqrt(pow(stops[e].row - stops[i].row, 2) + pow(stops[e].col - stops[i].col, 2));
	}

	for (i = 1; i < map_size_rows - 1; i++) {
		for (j = 1; j < map_size_cols - 1; j++) {
			if (ind[i][j] >= 0) {
				for (k = i - 1; k <= i + 1; k++) {        //these are
					for (l = j - 1; l <= j + 1; l++) {    //the 8 directions = matrix needs padding
						if ((k == i) && (l == j)) {       //and the eye
							continue;
						}

						//don't look at corners
						if ((k == i - 1) && (l == j - 1)) { continue; }
						if ((k == i - 1) && (l == j + 1)) { continue; }
						if ((k == i + 1) && (l == j - 1)) { continue; }
						if ((k == i + 1) && (l == j + 1)) { continue; }

						if (ind[k][l] >= 0) {			  
							++r_len;
							//routes = (struct route*)realloc(routes, r_len * sizeof(struct route));
							//printf("routes: %i \n", sizeof(*routes));

							int t = r_len - 1;
							routes[t].x = ind[i][j];
							routes[t].y = ind[k][l];
							//routes[t].d = sqrt(pow(stops[routes[t].y].row - stops[routes[t].x].row, 2) + pow(stops[routes[t].y].col - stops[routes[t].x].col, 2));
							routes[t].d = pow(stops[routes[t].y].row - stops[routes[t].x].row, 2) + pow(stops[routes[t].y].col - stops[routes[t].x].col, 2);
							//routes[t].d = abs(stops[routes[t].y].row - stops[routes[t].x].row) + abs(stops[routes[t].y].col - stops[routes[t].x].col);
							

							++stops[routes[t].x].n_len;
							//stops[routes[t].x].n = (int*)realloc(stops[routes[t].x].n, stops[routes[t].x].n_len * sizeof(int));

							//printf("%i = %i \n", routes[t].x, stops[routes[t].x].n_len);


							stops[routes[t].x].n[stops[routes[t].x].n_len - 1] = t;

							printf("t= %i \n", t);
						}
					}
				}
			}
		}
	}

	open[0] = s;
	stops[s].g = 0;
	stops[s].f = stops[s].g + stops[s].h;
	found = 0;

	while (o_len and not found) {
		min = DBL_MAX;

		for (i = 0; i < o_len; i++) {
			if (stops[open[i]].f < min) {
				current = open[i];
				min = stops[open[i]].f;
			}
		}

		if (current == e) {
			found = 1;

			++p_len;
			//path = (int*)realloc(path, p_len * sizeof(int));
			path[p_len - 1] = current;
			while (stops[current].from >= 0) {
				current = stops[current].from;
				++p_len;
				//path = (int*)realloc(path, p_len * sizeof(int));
				path[p_len - 1] = current;
			}
		}

		for (i = 0; i < o_len; i++) {
			if (open[i] == current) {
				if (i not_eq (o_len - 1)) {
					for (j = i; j < (o_len - 1); j++) {
						open[j] = open[j + 1];
					}
				}
				--o_len;
				//open = (int*)realloc(open, o_len * sizeof(int));
				break;
			}
		}

		++c_len;
		//closed = (int*)realloc(closed, c_len * sizeof(int));
		closed[c_len - 1] = current;

		for (i = 0; i < stops[current].n_len; i++) {
			b = 0;

			for (j = 0; j < c_len; j++) {
				if (routes[stops[current].n[i]].y == closed[j]) {
					b = 1;
				}
			}

			if (b) {
				continue;
			}

			tempg = stops[current].g + routes[stops[current].n[i]].d;

			b = 1;

			if (o_len > 0) {
				for (j = 0; j < o_len; j++) {
					if (routes[stops[current].n[i]].y == open[j]) {
						b = 0;
					}
				}
			}

			if (b or (tempg < stops[routes[stops[current].n[i]].y].g)) {
				stops[routes[stops[current].n[i]].y].from = current;
				stops[routes[stops[current].n[i]].y].g = tempg;
				stops[routes[stops[current].n[i]].y].f = stops[routes[stops[current].n[i]].y].g + stops[routes[stops[current].n[i]].y].h;

				if (b) {
					++o_len;
					//open = (int*)realloc(open, o_len * sizeof(int));
					open[o_len - 1] = routes[stops[current].n[i]].y;
				}
			}
		}
	}

	for (i = 0; i < map_size_rows; i++) {
		for (j = 0; j < map_size_cols; j++) {
			if (map[i][j]) {
				putchar(0xdb);
			}
			else {
				b = 0;
				for (k = 0; k < p_len; k++) {
					if (ind[i][j] == path[k]) {
						++b;
					}
				}
				if (b) {
					putchar('x');
				}
				else {
					putchar(' ');
				}
			}
		}
		putchar('\n');
	}

	if (not found) {
		puts("IMPOSSIBLE");
	}
	else {
		printf("path cost is %d:\n", p_len);
		for (i = p_len - 1; i >= 0; i--) {
			printf("(%i, %i)\n", stops[path[i]].col, stops[path[i]].row);
		}
	}

	printf("size of stop: %i bytes\n", sizeof(struct stop));
	printf("size of route: %i bytes\n", sizeof(struct route));
	printf("stops:  #%i %.2f kB\n", sizeof(stops)  / sizeof(struct stop),      sizeof(stops) / 1024.0);
	printf("routes: #%i %.2f kB\n", sizeof(routes) / sizeof(struct route),     sizeof(routes) / 1024.0);
	printf("path:   #%i %.2f kB\n", sizeof(path)   / sizeof(short int), sizeof(path) / 1024.0);
	printf("open:   #%i %.2f kB\n", sizeof(open)   / sizeof(short int), sizeof(open) / 1024.0);
	printf("closed: #%i %.2f kB\n", sizeof(closed) / sizeof(short int), sizeof(closed) / 1024.0);

	//for (i = 0; i < s_len; ++i) {
		//free(stops[i].n);
	//}
	//free(stops);
	//free(routes);
	//free(path);
	//free(open);
	//free(closed);

	return 0;
}