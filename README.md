# zero_dividend_injection

Simple repo to reproduce the Zero Divident Injection from the paper "Hide and Seek with Spectres: Efficient discovery of
speculative information leaks with random testing" by Oleksii Oleksenko, Marco Guarnieri, Boris Kopf and Mark
Silberstein.

[Link to the paper](https://arxiv.org/pdf/2301.07642.pdf)


## 1. Compile

```
source compile.sh
```

You might need to install `nasm` on your system before

## 2. Verify Flush+Reload works

```
./fr_checker
```

If you see that you can leak the string `pineapple-pizza` it means that you are good to go.
The goal of this tool is to verify the the macro `THR` is properly selected to be in the middle between a fast access
(cache hit) and a slow access (cache miss).

This is an example of a sane configuration:

```
Fast access: avg: 30.299999 min: 28 max: 32
Slow access: avg: 1010.599976 min: 136 max: 70460
Threshold: 160
```

More info on this tool [here](https://github.com/vusec/bhi-spectre-bhb/tree/main/tools/fr_checker).

## 3. Run ZDI

Run `zdi` and verify that you get a double hit on the flush & reload buffer (one architectural and one speculative):

```
./zdi
Test ZDI on 2^64 / 2^52
2^64 / 2^52 == 4096 (hit second fr_buf entry architecturally)
0    / 2^52 == 0    (hit first  fr_buf entry speculatively)
results[0] = 9999
results[1] = 9999
```


## Troubleshooting

* `performance` governor may help
* Double check `THR` value
* Make upper divided computation slower by increasing the `%rep 64`
