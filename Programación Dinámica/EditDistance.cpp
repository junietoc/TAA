class Solution {
public:
    int minDistance(string word1, string word2) {
        int n = word1.size();
        int m = word2.size();
        vector<vector<int>> matrix(n+1, vector<int>(m+1, 0));
        // llenar casos base
        for(int j = 0; j <= m; j++) matrix[n][j] = m-j;
        for(int i = 0; i <= n; i++) matrix[i][m] = n-i;

        // programación dinámica "bottom - up"
        for(int i = n-1; i >= 0; i--){
            for(int j = m-1; j >=0; j--){
                // son iguales
                if (word1[i] == word2[j]){
                    matrix[i][j] = matrix[i+1][j+1];
                }
                // revisar 3 operaciones
                else{
                    int insert_op = matrix[i+1][j];
                    int delete_op = matrix[i][j+1];
                    int replac_op = matrix[i+1][j+1];
                    
                    matrix[i][j] = 1 + min({insert_op, delete_op, replac_op}); 
                }
            }
        }
        return matrix[0][0];
    }
};