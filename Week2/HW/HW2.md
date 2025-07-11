# Homework2: 記述問題
　  
:::note warn
「自力で思いつくだけ書いてみよう」みたいな趣旨の宿題だったので、答えは調べずに進めています (取り組み方としては合っていますか？)　  
Week3の授業で解説を聞いてから、余裕のある時間に深掘りしようと思っています　  
:::
　  
## Problem Statement
> 木構造を使えば O(log N)、ハッシュテーブルを使えばほぼ O(1) で検索・追加・削除を実現することができて、これだけ見ればハッシュテーブルのほうが優れているように見える。　  
ところが現実の大規模なデータベースでは、ハッシュテーブルではなく木構造が使われることが多い。　  
その理由を考えよ。　  
　  
## My Answer
私は、大きく分けて以下の4つが理由になっていると考えました。　  
1. **木構造の方ができることが多い** (この意味では、個々の値のみを扱いたいのならハッシュテーブルでも構わない)　  
2. **ハッシュテーブルは最悪時間計算量がボリューミー**　  
3. **木構造の方が理解に要するコストが低い**　  
　  
### 1. 木構造の方ができることが多い
ユーザーネームとパスワードの組み合わせを保存するだけであれば、ハッシュテーブルで一つずつ取り出すのが最も効率的でしょう。　  
ハッシュテーブルはハッシュ値をindexとしてダイレクトに欲しい値を拾いに行けるので、こういった作業では木構造より優れていると言えます。　  
しかし現実の大規模なデータベースでは、格納されているデータを使って統計処理を施したり、データベースの中をさらにグループ分けしたり、**単なるCRUDだけでなく複雑な処理を行うことが多い** のではないでしょうか。　  
実装時にそのつもりがなくても、後から気が変わる可能性だってあります。　  
　  
そして **こういった処理においては、ハッシュテーブルよりも木構造の方が優れています。**　  
例えば、セグ木を使えば区間に対してまとめて処理がしやすくなるでしょう。Union Find木を使えばグループの結合や所属判定もしやすいです。　  
こういった **現実のプロダクトにおける操作の複雑性** は、少なからず木構造の人気の一因になっているのではと考えます。　  
　  
### 2. ハッシュテーブルは最悪時間計算量がボリューミー
これは授業で扱った内容かと思います。　  
　  
ハッシュテーブルは最悪時間計算量がO(N)あります。これは衝突が起こりうる以上仕方のないことです。　  
対して、**二分探索バランス木を実装すれば最悪時間計算量もO(logN)で済みます。**　  
　  
プロダクトは会社の名前を背負って世に出ます。期待値だけではなく、最悪のケースも想定されなければいけません。　  
こういった意味では、時間計算量･空間計算量ともに運任せな部分のあるハッシュテーブルよりも、安定してパフォーマンスを発揮できる木構造の方が好まれるのではと思いました。　  
　  
### 3. 木構造の方が理解に要するコストが低い
(こちらの答えは、個人の意見を大いに含んでいる気がします...)　  
　  
これはソースとして不十分ですが、私の読んできた本([鉄則本](https://www.amazon.co.jp/%E7%AB%B6%E6%8A%80%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0%E3%81%AE%E9%89%84%E5%89%87-%E3%82%A2%E3%83%AB%E3%82%B4%E3%83%AA%E3%82%BA%E3%83%A0%E5%8A%9B%E3%81%A8%E6%80%9D%E8%80%83%E5%8A%9B%E3%82%92%E9%AB%98%E3%82%81%E3%82%8B77%E3%81%AE%E6%8A%80%E8%A1%93-Compass-Books%E3%82%B7%E3%83%AA%E3%83%BC%E3%82%BA-%E7%B1%B3%E7%94%B0/dp/483997750X), [蟻本](https://www.amazon.co.jp/%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0%E3%82%B3%E3%83%B3%E3%83%86%E3%82%B9%E3%83%88%E3%83%81%E3%83%A3%E3%83%AC%E3%83%B3%E3%82%B8%E3%83%96%E3%83%83%E3%82%AF-%E7%AC%AC2%E7%89%88-%EF%BD%9E%E5%95%8F%E9%A1%8C%E8%A7%A3%E6%B1%BA%E3%81%AE%E3%82%A2%E3%83%AB%E3%82%B4%E3%83%AA%E3%82%BA%E3%83%A0%E6%B4%BB%E7%94%A8%E5%8A%9B%E3%81%A8%E3%82%B3%E3%83%BC%E3%83%87%E3%82%A3%E3%83%B3%E3%82%B0%E3%83%86%E3%82%AF%E3%83%8B%E3%83%83%E3%82%AF%E3%82%92%E9%8D%9B%E3%81%88%E3%82%8B%EF%BD%9E-%E7%A7%8B%E8%91%89%E6%8B%93%E5%93%89/dp/4839941068), [螺旋本](https://www.amazon.co.jp/%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0%E3%82%B3%E3%83%B3%E3%83%86%E3%82%B9%E3%83%88%E6%94%BB%E7%95%A5%E3%81%AE%E3%81%9F%E3%82%81%E3%81%AE%E3%82%A2%E3%83%AB%E3%82%B4%E3%83%AA%E3%82%BA%E3%83%A0%E3%81%A8%E3%83%87%E3%83%BC%E3%82%BF%E6%A7%8B%E9%80%A0-%E6%B8%A1%E9%83%A8-%E6%9C%89%E9%9A%86/dp/4839952957/ref=pd_lpo_d_sccl_2/355-0996745-6487862?pd_rd_w=yC0YY&content-id=amzn1.sym.855d8f70-df76-4181-80b0-56e48ae3bb9b&pf_rd_p=855d8f70-df76-4181-80b0-56e48ae3bb9b&pf_rd_r=64EFDVRQY1E0ZN51FXPR&pd_rd_wg=GfSqM&pd_rd_r=e1691da3-0957-400e-aee2-f77f7e5069b4&pd_rd_i=4839952957&psc=1), etc...)では　  
- 木構造は実装しても、ハッシュや連想配列に関しては既存のライブラリを使うことがほとんどでした　  
- 書籍内での登場順も、木構造の方が早かったです　  
　  
よって、木構造の方が一般的に可読性の高いアルゴリズムなのかな、と思っていました。　  
　  
実際のプロダクトは長期にわたってメンテナンスをする必要があります。またプロジェクトにはたくさんの人が関わるでしょう。　  
作業の効率性が重要になってくる業務では、よりわかりやすいアルゴリズムの方が好まれそうな気がします。　  