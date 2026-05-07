#!/bin/bash
# このスクリプトはリポジトリのトップレベルのディレクトリで実行すること

echo "パーサの動作確認"
gcc parse.c value.c test/parse-test.c && ./a.out

echo "valueの動作確認"
gcc test/value-test.c value.c env.c && ./a.out

rm ./a.out

echo ""
