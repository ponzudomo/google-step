# Homework3: 記述問題
　  
:::note warn
「自力で思いつくだけ書いてみよう」みたいな趣旨の宿題だったので、答えは調べずに進めています (取り組み方としては合っていますか？)　  
Week3の授業で解説を聞いてから、余裕のある時間に深掘りしようと思っています　  
:::
　  
## Problem Statement
> 目標：「もっとも直近にアクセスされた上位 X 個の (URL, Web ページ) の組が保存できるデータ構造」を作る　
以下の操作がほぼ O(1) で実現できるようなデータ構造を考える　  
> - 与えられた (URL, Web ページ) があるかないかを検索　  
> - もしない場合、キャッシュ内で一番古い (URL, Web ページ) を捨てて、代わりに与えられた (URL, Web ページ) を追加する  
　  
## My Answer
### 基本方針
- CREATEとDELETEの流れが常に一方通行なので、基本的にはqueueを使った実装がいいのかな と思っています　  
- queueでは基本的に両端しか見れないので、同じ情報が重複して登録されてしまい、消す必要のない情報が消されてしまう可能性があります
以下のアイデアはどうでしょうか？　  
- ハッシュテーブルとqueueの組み合わせ　  
- ハッシュテーブルと一緒に記録された時間を管理(更新する必要がない)　  
- ハッシュテーブルと一緒に登場回数countを管理(queueのお尻から出てきた時に、countから引いていく)　  
　  

## Office Hourで話した内容
- 例えばCが連続して100個入力されたとき、queueもcountもどんどん膨らんでいってしまう
- データ構造の大きさもカウントもコンパクトであればコンパクトであるほど嬉しいので、連結リストでどんどん消していった方がいい
- ポインタを保存しておいて、そのポインタの指し示す値を削除、前後を繋ぎ合わせて無かったことにする
- お尻から新しくcを入れる

## harakenさんによる答え合わせ
LRU(Least Recently Used) と呼ばれるアルゴリズムらしい