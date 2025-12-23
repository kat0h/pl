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
