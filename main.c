#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_STATES 100
#define MAX_TRANS 100
#define MAX_ALPHABET 26
#define START_STATE 97

int char_to_num(char ch) {
    return (int)ch - 97;
}

typedef struct {
    int num_a, num_s, init_s, num_f;
    int final_ss[MAX_STATES];
    int trans_to[MAX_STATES][MAX_ALPHABET][MAX_STATES];
    int trans_counts[MAX_STATES][MAX_ALPHABET];
} DFA;

void init_DFA(DFA *dfa, int num_a, int num_s, int init_s, int num_f, int final_ss[], char trans_func[MAX_TRANS][3][100], int trans_count) {
    dfa->num_a = num_a;
    dfa->num_s = num_s;
    dfa->init_s = init_s;
    dfa->num_f = num_f;
    for (int i = 0; i < num_f; i++) {
        dfa->final_ss[i] = final_ss[i];
    }

    for (int i = 0; i < trans_count; i++) {
        int st1 = atoi(trans_func[i][0]);
        char alph = trans_func[i][1][0];
        int st2 = atoi(trans_func[i][2]);
        dfa->trans_to[st1][char_to_num(alph)][dfa->trans_counts[st1][char_to_num(alph)]] = st2;
        dfa->trans_counts[st1][char_to_num(alph)]++;
    }
}

void print_transactions(DFA *dfa) {
    for (int i = 0; i < dfa->num_s; i++) {
        for (int j = 0; j < dfa->num_a; j++) {
            for (int k = 0; k < dfa->trans_counts[i][j]; k++) {
                printf("%d %c %d\n", i, START_STATE + j, dfa->trans_to[i][j][k]);
            }
        }
    }
}

bool is_final_state(DFA *dfa, int state) {
    for (int i = 0; i < dfa->num_f; i++) {
        if (dfa->final_ss[i] == state) {
            return true;
        }
    }
    return false;
}

bool already_visited(int visited[][3], int v, int i, int st2, int count) {
    for (int k = 0; k < count; k++) {
        if (visited[k][0] == v && visited[k][1] == i && visited[k][2] == st2) {
            return true;
        }
    }
    return false;
}

void dfs(DFA *dfa, int v, int visited[MAX_TRANS][3], int result[MAX_ALPHABET], int result_len, int visited_count) {
    if (is_final_state(dfa, v)) {
        for (int i = 0; i < result_len; i++) {
            printf("%c", result[i] + START_STATE);
        }
        printf("\n");
    }

    for (int i = 0; i < dfa->num_a; i++) {
        for (int j = 0; j < dfa->trans_counts[v][i]; j++) {
            int st2 = dfa->trans_to[v][i][j];
            if (!already_visited(visited, v, i, st2, visited_count)) {
                visited[visited_count][0] = v;
                visited[visited_count][1] = i;
                visited[visited_count][2] = st2;
                result[result_len] = i;
                dfs(dfa, st2, visited, result, result_len + 1, visited_count + 1);
            }
        }
    }
}

void get_all_words(DFA *dfa) {
    int visited[MAX_TRANS][3] = {0};
    int result[MAX_ALPHABET] = {0};
    dfs(dfa, dfa->init_s, visited, result, 0, 0);
}

int main() {
    FILE *file = fopen("input.txt", "r");
    int num_a, num_s, init_s, num_f;
    fscanf(file, "%d %d %d %d", &num_a, &num_s, &init_s, &num_f);

    int final_ss[MAX_STATES];
    for (int i = 0; i < num_f; i++) {
        fscanf(file, "%d", &final_ss[i]);
    }

    char trans_func[MAX_TRANS][3][100];
    int trans_count = 0;
    while (fscanf(file, "%s %s %s", trans_func[trans_count][0], trans_func[trans_count][1], trans_func[trans_count][2]) != EOF) {
        trans_count++;
    }
    fclose(file);

    DFA dfa;
    init_DFA(&dfa, num_a, num_s, init_s, num_f, final_ss, trans_func, trans_count);

    printf("Initial transactions:\n");
    print_transactions(&dfa);

    printf("All possible words (patterns included 1 time): \n");
    get_all_words(&dfa);

    return 0;
}
