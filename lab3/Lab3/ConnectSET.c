
#include "ConnectSET.h"


void connectNeighbors(
	struct pixel s,
	double T,
	unsigned char **img,
	int width,
	int height,
	int *M,
	struct pixel c[4]) {
	*M = 0;
	if ((s.x - 1 >= 0) && abs(img[s.x][s.y] - img[s.x - 1][s.y]) <= T) {
		c[*M].x = s.x - 1;
		c[*M].y = s.y;
		(*M)++;
	}
	if ((s.x + 1 < height) && abs(img[s.x][s.y] - img[s.x + 1][s.y]) <= T) {
		c[*M].x = s.x + 1;
		c[*M].y = s.y;
		(*M)++;
	}
	if ((s.y - 1 >= 0) && abs(img[s.x][s.y] - img[s.x][s.y - 1]) <= T) {
		c[*M].x = s.x;
		c[*M].y = s.y - 1;
		(*M)++;
	}
	if ((s.y + 1 < width) && abs(img[s.x][s.y] - img[s.x][s.y + 1]) <= T) {
		c[*M].x = s.x;
		c[*M].y = s.y + 1;
		(*M)++;
	}
}

void connectSet(
	struct pixel s,
	double T,
	unsigned char **img,
	int width,
	int height,
	int ClassLabel,
	unsigned int **seg,
	int *numCpixels) {
	*numCpixels = 0;
	struct pixel c[4];
	struct Node *head, *temp, *last,*temp2;
	int M;                                                      //futher modification.
	head = (struct Node *)malloc(sizeof(struct Node));
	head->current.x = s.x;
	head->current.y = s.y;
	head->next = NULL;
	last = head;
	int WLpixel;
	WLpixel = 1;
	while(WLpixel != 0){
		if (seg[head->current.x][head->current.y] != ClassLabel) {
			seg[head->current.x][head->current.y] = ClassLabel;
			(*numCpixels)++;
			connectNeighbors(head->current, T, img, width, height, &M, c);
			for (int i = 0; i < M; i++) {
				if (seg[c[i].x][c[i].y] != ClassLabel) {    
					// here use queue data structure to store the pixels waiting for processing
					temp = (struct Node *)malloc(sizeof(struct Node));  
					temp->current.x = c[i].x;
					temp->current.y = c[i].y;
					temp->next = NULL;    //enqueue
					WLpixel++;          //keep the number of the unprocessed pixels

					last->next = temp;
					last = temp;
				}
			}
		}

		// After processing, Dequeue
		temp2 = head->next;
		free(head);
		head = temp2;
		WLpixel--;
	}
}