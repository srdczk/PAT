class Solution {
public:

    bool canPartition(vector<int>& nums) {
        int sum = 0;
        for (auto &num : nums) sum += num;
        if (sum % 2) return false;
        vector<bool> dp(sum / 2 + 1, false);
        dp[0] = true;
        for (auto &num : nums) {
            for (int i = sum / 2; i > num - 1; --i) 
                if (dp[i - num]) dp[i] = true;
        }
        return dp[sum / 2];
    }
};