# Schemeの小さなサブセットのインタプリタ

これはR5RSを元に制作しているSchemeの小さなサブセットのインタプリタである。
現在、下記のような機能を実装している。

- 四則演算
- `set!`による変数への値の代入
- `lambda`関数による第一級関数の生成
- `if`/`cond`による処理の分岐
- `call/cc`による第一級継続の取得・呼び出し

## ビルド方法

```
$ make
```

## サンプルプログラムの実行

次のようにサンプルプログラムを実行できる。

```
./scm "`cat ./sample/callcc.scm`"  # call/ccで10までカウント
./scm "`cat ./sample/anonymous_recursion.scm`" # lambda式のみで1~10までの値の合計を計算
./scm "`cat ./sample/counter.scm`" # クロージャのサンプル
./scm "`cat ./sample/fizzbuzz.scm`" # FizzBuzz
./scm "`cat ./sample/lambda.scm`" # lambda式のサンプル
./scm "`cat ./sample/takeuchi.scm`" # 竹内関数
./scm "`cat ./sample/generator.scm`" # call/ccでジェネレータを実装
./scm "`cat ./sample/corutine.scm`" # call/ccで対照コルーチンを実装
./scm "`cat ./sample/callcc_goto.scm`" # call/ccでgotoを模倣
./scm "`cat ./sample/power.scm`" # call/ccでべき乗を計算
```

## テストについて

下記のコマンドでテストを実行できる。
このプログラムを書き始めた際はテストの環境を整えていなかったため、簡易的な退行テストとしてサンプルプログラムが全てエラーなしに終了するかをテストしている。

```
$ make test
```

退行テストのスクリプトとtest_runnerが正常に終了すればテストに通過している。

## TDDの実践

TDDの実践の記録について下記に示す。

演習ではインタプリタのパーサに`` `(1 2 3) `` のような記法を実装することにした。
この記法は、`(quote (1 2 3))`の糖衣構文とする。

よって、始めにテストとして`test.c`に`test_quote_equivalence`関数を追加した。二つの入力が一致するかをチェックする。

```
void test_quote_equivalence() {
    // (quote (1 2 3)) and '(1 2 3) should be parsed to the same structure
    value *a = parse_program("(quote (1 2 3))");
    value *b = parse_program("`(1 2 3)");
    assert(value_equal(a, b));
}
```

この段階での`make test`の実行結果は下記の通りで、テストが失敗している。

```
./sample/test.sh
Building the 'scm' interpreter...
make[1]: Entering directory '/home/kat0h/ghq/github.com/kat0h/pl/scheme'
make[1]: Nothing to be done for 'all'.
make[1]: Leaving directory '/home/kat0h/ghq/github.com/kat0h/pl/scheme'
Starting regression tests for sample Scheme programs...
RUNNING TEST: sample/anonymous_recursion.scm
~~ 省略 ~~
PASSED TEST: sample/takeuchi.scm
All regression tests passed successfully!
./test_runner
main.c:339 of parse_paren: Unexpected token '
make: *** [Makefile:21: test] Error 1
```

次に、このテストを通過するようにパーサのプログラムを変更することとした。
しかし、実装したところ、`parse_program()`はトップレベルに括弧で囲われたものしか想定していないことが分かった。
そこで、一旦`test.c`を書き換え、次のようにした。

```
void test_quote_equivalence() {
    // (quote (1 2 3)) and '(1 2 3) should be parsed to the same structure
    value *a = parse_program("(print (quote (1 2 3)))");
    value *b = parse_program("(print `(1 2 3))");
    assert(value_equal(a, b));
}
```

この変更により、書き換えたパーサのコードがテストに通過した。


次に、変更についてリファクタリングをした。
現状、コードは全て`main.c`に実装が纏まっているため、パーサの部分を別に分けることにした。

新しく`parse.h`と`parse.c`を作成し、`main.c`からパーサの実装を移動した。テストがあるため、コードを大きく変更しているものの、退行が起こっていないことを容易に検証できた。

以上でTDDによる一つのステップが終了した。