/**
 * @file
 * broccoli.cpp
 * 
 * @brief
 * ブロッコリーとカリフラワーがたくさん並んでいる。
 * 全てのブロッコリーが全てのカリフラワーの左にある状態を「美しいブロッコリー列」と呼ぶ
 * あなたはブロッコリーをカリフラワーに、またはカリフラワーをブロッコリーに、一つずつ置き換えることができる
 * 最小で、何回の置換で美しいブロッコリー列が作れる？
 * 入力はGreen/Writeの頭文字をとって、GWWWWGWWGGGGWGのような文字列で与えられる
 * 
 * @note
 * 解法1 : 貪欲 O(n)
 *  - GWのstring列を01のint列に変換し、累積和をとる 
 *    - メモリと時間は食うが、ブロとカリで2種類用意しておくと読みやすいかも
 *  - ブロ/カリの切り替え地点を全探索、累積和から置き換え回数を求めていき最小値を特定 
 * 
 * 解法2 : 上手く行けば計算量が減りそうな再帰 O(n)
 *  - 左端(left = 0)と右端(right = n-1)からGWの数をそれぞれカウントしていく
 *  - 左側から見ていった時、Gの数がWの数を上回った瞬間、そこまでの部分はGに揃えたほうが効率的であると言える
 *    - そこまでの部分を切り落とし(left = i+1 に更新)、ans += left_W、左側のGWのカウントを0で初期化
 *  - 右についても同じことをする
 *  - indexが交差したら終了　leftとrightの間が空いていれば再帰的に同じ処理を行う
 *    - 無限ループを防ぐため、再帰ステップの中で前に進まなくなるようであれば内部で解法1をとる
 * 
 * 解法3 : DP O(n)
 *  - dp[i][j] : i番目まで見た時、最後の文字がjであった時の最小置換回数
 *  - dp[i][G] = dp[i-1][G] + (s[i] == G ? 0 : 1);
 *  - dp[i][W] = min(dp[i-1][G], dp[i-1][W]) + (s[i] == W ? 0 : 1);
 *  - ans = min(dp[n-1][G], dp[n-1][W]);
 * 
 * 解法1は、愚直ながら切り替わり地点が分かるというメリットがある
 * 解法2はABC408_Dでは有効かもしれないけど、この問題で使う必要はあんまりないかも
 * 解法3は切り替わり地点などの情報が潰れてしまう代わりに、コスパが良さそう
 * 今回は置換回数のみ求めれば良いので、解法3を使うことにする
 * 
 */


// include
#include <bits/stdc++.h>
using namespace std;
struct Init { Init() { ios::sync_with_stdio(0); cin.tie(0); } }init;

int main() {
    string input;
    cin >> input;
    /// @brief dp[i][j] : i番目まで見た時、最後の文字がjであった時の最小置換回数 (G=0, W=1とする)
    vector<vector<int>> dp(input.size(), vector<int>(2, 0)); // 一応明示的に0で初期化
    dp[0][(input[0] == 'G')]++; // 最初の文字がGならdp[0][1]を1、Wならdp[0][0]を1にする
    for(int i=1; i<input.size(); i++) {
        dp[i][0] = dp[i-1][0] + (input[i] == 'W'); // 最後の文字がWなら置換回数を1増やす
        dp[i][1] = min(dp[i-1][0], dp[i-1][1]) + (input[i] == 'G'); // 最後の文字がGなら置換回数を1増やす  
    }
    cout << min(dp[input.size()-1][0], dp[input.size()-1][1]) << endl; // 最後の文字がGかWかで最小置換回数を出力
    return 0;
}