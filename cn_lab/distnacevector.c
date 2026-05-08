#include <stdio.h>

void main() {
  int n;
  printf("Enter the number of nodes: ");
  scanf("%d", &n);

  int dist[n][n], cost[n][n];
  printf("Enter the Matrix (999) for infinity \n");
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      scanf("%d", &cost[i][j]);
      dist[i][j] = cost[i][j];
    }
  }

  for (int k = 0; k < n; ++k) {
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        if (dist[i][j] > dist[k][j] + dist[i][k]) {
          dist[i][j] = dist[k][j] + dist[i][k];
        }
      }
    }
  }

  printf("\nRouting Tables\n");
  for (int i = 0; i < n; ++i) {
    printf("Router %d \n", i + 1);
    for (int j = 0; j < n; ++j) {
      printf("Destination %d cost: %d \n", j + 1, dist[i][j]);
    }
    printf("\n");
  }
}
