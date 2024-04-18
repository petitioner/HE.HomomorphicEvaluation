# 並行計算時的Bug是scheme.addAndEqual(outputCT, tempCTs[inputidx]);NTL並行計算不完美應該尋找替代方案：NTL並行塊里可能無法對共享變量進行改變
    ```cpp
       ZZ InnerProd(const ZZ *a, const ZZ *b, long n)
   {
      PartitionInfo pinfo(n);

      long cnt = pinfo.NumIntervals();

      Vec<ZZ> acc;
      acc.SetLength(cnt);

      NTL_EXEC_INDEX(cnt, index)

         long first, last;
         pinfo.interval(first, last, index);

         ZZ& sum = acc[index];
         sum = 0;
         for (long i = first; i < last; i++)
            MulAddTo(sum, a[i], b[i]);

      NTL_EXEC_INDEX_END

      ZZ sum;
      sum = 0;
      for (long i = 0; i < cnt; i++)
         sum += acc[i];

      return sum;
   }
   ```
   from https://libntl.org/doc/BasicThreadPool.cpp.html

# 可以使用NN來找最佳多項式 畢竟NN矩陣乘法多項式激活函數的結果就是多項式

# HE.HomomorphicComparison

```cpp
root@vultr:~# cd ..
root@vultr:/# cd home/
root@vultr:/home# mkdir sunly
root@vultr:/home# cd sunly/
root@vultr:/home/sunly# mkdir Downloads
root@vultr:/home/sunly# cd Downloads/
root@vultr:/home/sunly/Downloads# git clone https://github.com/petitioner/HE.HomomorphicComparison
Cloning into 'HE.HomomorphicComparison'...
remote: Enumerating objects: 397, done.
remote: Counting objects: 100% (11/11), done.
remote: Compressing objects: 100% (11/11), done.
remote: Total 397 (delta 0), reused 9 (delta 0), pack-reused 386
Receiving objects: 100% (397/397), 23.51 MiB | 43.92 MiB/s, done.
Resolving deltas: 100% (123/123), done.
root@vultr:/home/sunly/Downloads# mv HE.HomomorphicComparison/ HomomorphicComparison-master
root@vultr:/home/sunly/Downloads# cd HomomorphicComparison-master/HomomorphicComparison/lib/lib/
root@vultr:/home/sunly/Downloads/HomomorphicComparison-master/HomomorphicComparison/lib/lib# ls
libgmp.a  libgmpxx.a  libntl.a.zip  README.md
root@vultr:/home/sunly/Downloads/HomomorphicComparison-master/HomomorphicComparison/lib/lib# unzip libntl.a.zip 
Archive:  libntl.a.zip
  inflating: libntl.a                
root@vultr:/home/sunly/Downloads/HomomorphicComparison-master/HomomorphicComparison/lib/lib# cd ..
root@vultr:/home/sunly/Downloads/HomomorphicComparison-master/HomomorphicComparison/lib# cd ..
root@vultr:/home/sunly/Downloads/HomomorphicComparison-master/HomomorphicComparison# cd Default/
root@vultr:/home/sunly/Downloads/HomomorphicComparison-master/HomomorphicComparison/Default# make clean && make all

Finished building target: MyHomomorphicComparison
 
root@vultr:/home/sunly/Downloads/HomomorphicComparison-master/HomomorphicComparison/Default# ./MyHomomorphicComparison 
!!! START TEST BASIC !!!
!!! START TEST BASIC !!!


Test cosx form exp(ix)
(0.93154,0) = = 0.932318
(0.866501,0) = = 0.869445
(0.91655,0) = = 0.917704
(0.999271,0) = = 0.999272
(0.532406,0) = = 0.567729
(0.937187,0) = = 0.937842
(0.545131,0) = = 0.578586
(0.970774,0) = = 0.970916
(0.731457,0) = = 0.743263
(0.985032,0) = = 0.985069
(0.941244,0) = = 0.941817
(0.648729,0) = = 0.668819
(0.99955,0) = = 0.99955
(0.830183,0) = = 0.834935
(0.971635,0) = = 0.971768
(0.767189,0) = = 0.776083
(0.884162,0) = = 0.886381
(0.880943,0) = = 0.883287
(0.50487,0) = = 0.544404
(0.665975,0) = = 0.684161
(0.887695,0) = = 0.889781
(0.596674,0) = = 0.623067
(0.969978,0) = = 0.970128
(0.74123,0) = = 0.752199
(0.998596,0) = = 0.998596
(0.633706,0) = = 0.655529
(0.889312,0) = = 0.891339
(0.565024,0) = = 0.595657
(0.836452,0) = = 0.840862
(0.993793,0) = = 0.9938
(0.941063,0) = = 0.941639
(0.556365,0) = = 0.588212
(0.802725,0) = = 0.809127
(0.717351,0) = = 0.730418
(0.519682,0) = = 0.556922
(0.822878,0) = = 0.828046
(0.99435,0) = = 0.994355
(0.563894,0) = = 0.594685
(0.649755,0) = = 0.669729
(0.647904,0) = = 0.668087
(0.994282,0) = = 0.994287
(0.983845,0) = = 0.983888
(0.770606,0) = = 0.779243
(0.990624,0) = = 0.990638
(0.709272,0) = = 0.723089
(0.58091,0) = = 0.609377
(0.664368,0) = = 0.682728
(0.970266,0) = = 0.970413
(0.918597,0) = = 0.919696
(0.668356,0) = = 0.686287
(0.998127,0) = = 0.998128
(0.615065,0) = = 0.639136
(0.746127,0) = = 0.756689
(0.953108,0) = = 0.953473
(0.8219,0) = = 0.827124
(0.706964,0) = = 0.720999
(0.986853,0) = = 0.986882
(0.997733,0) = = 0.997734
(0.756212,0) = = 0.765958
(0.730564,0) = = 0.742448
(0.984025,0) = = 0.984067
(0.999135,0) = = 0.999135
(0.771493,0) = = 0.780064
(0.674474,0) = = 0.691756


Test sinx form exp(ix)
(0.361583,0) = = 0.36164
(0.493724,0) = = 0.494029
(0.39717,0) = = 0.397264
(0.0381617,0) = = 0.0381617
(0.816322,0) = = 0.823215
(0.347016,0) = = 0.347062
(0.809184,0) = = 0.815621
(0.239414,0) = = 0.239421
(0.66726,0) = = 0.669
(0.172157,0) = = 0.172158
(0.336086,0) = = 0.336125
(0.740035,0) = = 0.743425
(0.0299933,0) = = 0.0299933
(0.549792,0) = = 0.550348
(0.235931,0) = = 0.235937
(0.629411,0) = = 0.630631
(0.462742,0) = = 0.462956
(0.468604,0) = = 0.468833
(0.830881,0) = = 0.838823
(0.726339,0) = = 0.729331
(0.456189,0) = = 0.456388
(0.777391,0) = = 0.782168
(0.242587,0) = = 0.242594
(0.65735,0) = = 0.658936
(0.0529661,0) = = 0.0529661
(0.751408,0) = = 0.75517
(0.453147,0) = = 0.453338
(0.797477,0) = = 0.803239
(0.540744,0) = = 0.54125
(0.111183,0) = = 0.111184
(0.336584,0) = = 0.336624
(0.802656,0) = = 0.808706
(0.586827,0) = = 0.587634
(0.681025,0) = = 0.683001
(0.823197,0) = = 0.830565
(0.560043,0) = = 0.560661
(0.1061,0) = = 0.1061
(0.79816,0) = = 0.803959
(0.73924,0) = = 0.742606
(0.740673,0) = = 0.744083
(0.106739,0) = = 0.106739
(0.178784,0) = = 0.178786
(0.625547,0) = = 0.626722
(0.136513,0) = = 0.136513
(0.688636,0) = = 0.690755
(0.787626,0) = = 0.792881
(0.727645,0) = = 0.730673
(0.241443,0) = = 0.24145
(0.392543,0) = = 0.392632
(0.724392,0) = = 0.727331
(0.0611656,0) = = 0.0611656
(0.764839,0) = = 0.769094
(0.652263,0) = = 0.653775
(0.301456,0) = = 0.301478
(0.561394,0) = = 0.562019
(0.690775,0) = = 0.692936
(0.161445,0) = = 0.161446
(0.0672814,0) = = 0.0672814
(0.641523,0) = = 0.642891
(0.66815,0) = = 0.669903
(0.177794,0) = = 0.177796
(0.041583,0) = = 0.041583
(0.624536,0) = = 0.6257
(0.719325,0) = = 0.722131
SDF
------------------
Start Encrypt two batch
Encrypt two batch time = 126.788 ms
------------------
ct1.logq = 1110		ct1.logp = 30
ct2.logq = 1110		ct2.logp = 30
ct3.logq = 1110		ct3.logp = 30
ct1.logq = 1110		ct1.logp = 30
ct2.logq = 1110		ct2.logp = 30
ct3.logq = 1110		ct3.logp = 30
------------------
Start Decrypt batch
Decrypt batch time = 18.321 ms
------------------
mvec1[i]		sin(mvec1[i])		dvec1[i]
8.14059		0.959208		0.959191
11.3678		-0.931524		-0.931481
8.98775		0.423252		0.423257
0.839761		0.744484		0.744481
21.2751		0.656381		0.656027
7.79761		0.998412		0.998412
20.9837		0.845498		0.845073
5.31892		-0.82163		-0.821636
16.123		-0.403189		-0.403134
3.80644		-0.616941		-0.616948
7.5416		0.951603		0.9516
18.4399		-0.398293		-0.398172
0.659953		0.613079		0.613069
12.8212		0.25206		0.252053
5.24002		-0.864004		-0.864003
15.012		0.641094		0.641018
10.5892		-0.918557		-0.918523
10.7353		-0.966328		-0.966293
21.8926		0.0983889		0.0982873
17.9816		-0.763035		-0.762825
10.4265		-0.842392		-0.842367
19.759		0.789185		0.788905
5.39085		-0.778537		-0.778542
15.8269		-0.118621		-0.1186
1.1658		0.919104		0.91911
18.8301		-0.0194662		-0.0194336
10.3512		-0.799447		-0.799426
20.5197		0.995071		0.994639
12.5823		0.0159319		0.0159372
2.45111		0.636913		0.636911
7.55323		0.955116		0.955111
20.7229		0.954583		0.954137
13.8189		0.949778		0.949682
16.541		-0.739965		-0.739833
21.5627		0.415502		0.415247
13.094		0.503515		0.503466
2.3386		0.719441		0.719432
20.5463		0.992078		0.991633
18.413		-0.422863		-0.42272
18.4616		-0.378336		-0.378214
2.35275		0.709539		0.709528
3.95455		-0.726324		-0.726327
14.9015		0.721873		0.721774
3.0127		0.128535		0.128554
16.7757		-0.876126		-0.875956
20.1415		0.961365		0.960983
18.0247		-0.734421		-0.734225
5.36492		-0.794548		-0.794536
8.87681		0.520956		0.520946
17.9173		-0.802941		-0.802721
1.34648		0.974947		0.974947
19.3033		0.438327		0.438188
15.6764		0.0315795		0.0315563
6.73734		0.4387		0.438696
13.1302		0.53439		0.534351
16.8422		-0.906202		-0.906023
3.56743		-0.413079		-0.41308
1.48131		0.995999		0.995992
15.3619		0.339239		0.339178
16.1497		-0.427536		-0.427458
3.93241		-0.710931		-0.710923
0.915091		0.792618		0.792618
14.8726		0.741526		0.741436
17.7513		-0.89041		-0.890171
---------------------
msin: 0 :0.959208
dsin: 0 :0.959191
esin: 0 :1.69931e-05
---------------------
---------------------
msin: 1 :-0.931524
dsin: 1 :-0.931481
esin: 1 :-4.26819e-05
---------------------
---------------------
msin: 2 :0.423252
dsin: 2 :0.423257
esin: 2 :-4.30195e-06
---------------------
---------------------
msin: 3 :0.744484
dsin: 3 :0.744481
esin: 3 :2.55014e-06
---------------------
---------------------
msin: 4 :0.656381
dsin: 4 :0.656027
esin: 4 :0.000353909
---------------------
---------------------
msin: 5 :0.998412
dsin: 5 :0.998412
esin: 5 :-2.2667e-07
---------------------
---------------------
msin: 6 :0.845498
dsin: 6 :0.845073
esin: 6 :0.000424448
---------------------
---------------------
msin: 7 :-0.82163
dsin: 7 :-0.821636
esin: 7 :6.28151e-06
---------------------
---------------------
msin: 8 :-0.403189
dsin: 8 :-0.403134
esin: 8 :-5.52237e-05
---------------------
---------------------
msin: 9 :-0.616941
dsin: 9 :-0.616948
esin: 9 :6.62983e-06
---------------------
---------------------
msin: 10 :0.951603
dsin: 10 :0.9516
esin: 10 :3.20277e-06
---------------------
---------------------
msin: 11 :-0.398293
dsin: 11 :-0.398172
esin: 11 :-0.000120334
---------------------
---------------------
msin: 12 :0.613079
dsin: 12 :0.613069
esin: 12 :1.03294e-05
---------------------
---------------------
msin: 13 :0.25206
dsin: 13 :0.252053
esin: 13 :6.89269e-06
---------------------
---------------------
msin: 14 :-0.864004
dsin: 14 :-0.864003
esin: 14 :-1.21418e-06
---------------------
---------------------
msin: 15 :0.641094
dsin: 15 :0.641018
esin: 15 :7.59231e-05
---------------------
---------------------
msin: 16 :-0.918557
dsin: 16 :-0.918523
esin: 16 :-3.39192e-05
---------------------
---------------------
msin: 17 :-0.966328
dsin: 17 :-0.966293
esin: 17 :-3.44236e-05
---------------------
---------------------
msin: 18 :0.0983889
dsin: 18 :0.0982873
esin: 18 :0.000101613
---------------------
---------------------
msin: 19 :-0.763035
dsin: 19 :-0.762825
esin: 19 :-0.000210294
---------------------
---------------------
msin: 20 :-0.842392
dsin: 20 :-0.842367
esin: 20 :-2.47743e-05
---------------------
---------------------
msin: 21 :0.789185
dsin: 21 :0.788905
esin: 21 :0.000279566
---------------------
---------------------
msin: 22 :-0.778537
dsin: 22 :-0.778542
esin: 22 :5.54215e-06
---------------------
---------------------
msin: 23 :-0.118621
dsin: 23 :-0.1186
esin: 23 :-2.08194e-05
---------------------
---------------------
msin: 24 :0.919104
dsin: 24 :0.91911
esin: 24 :-6.308e-06
---------------------
---------------------
msin: 25 :-0.0194662
dsin: 25 :-0.0194336
esin: 25 :-3.26151e-05
---------------------
---------------------
msin: 26 :-0.799447
dsin: 26 :-0.799426
esin: 26 :-2.1675e-05
---------------------
---------------------
msin: 27 :0.995071
dsin: 27 :0.994639
esin: 27 :0.000432206
---------------------
---------------------
msin: 28 :0.0159319
dsin: 28 :0.0159372
esin: 28 :-5.35303e-06
---------------------
---------------------
msin: 29 :0.636913
dsin: 29 :0.636911
esin: 29 :2.10316e-06
---------------------
---------------------
msin: 30 :0.955116
dsin: 30 :0.955111
esin: 30 :4.47835e-06
---------------------
---------------------
msin: 31 :0.954583
dsin: 31 :0.954137
esin: 31 :0.000445492
---------------------
---------------------
msin: 32 :0.949778
dsin: 32 :0.949682
esin: 32 :9.56302e-05
---------------------
---------------------
msin: 33 :-0.739965
dsin: 33 :-0.739833
esin: 33 :-0.000131635
---------------------
---------------------
msin: 34 :0.415502
dsin: 34 :0.415247
esin: 34 :0.000255122
---------------------
---------------------
msin: 35 :0.503515
dsin: 35 :0.503466
esin: 35 :4.88156e-05
---------------------
---------------------
msin: 36 :0.719441
dsin: 36 :0.719432
esin: 36 :9.51654e-06
---------------------
---------------------
msin: 37 :0.992078
dsin: 37 :0.991633
esin: 37 :0.000444837
---------------------
---------------------
msin: 38 :-0.422863
dsin: 38 :-0.42272
esin: 38 :-0.000142883
---------------------
---------------------
msin: 39 :-0.378336
dsin: 39 :-0.378214
esin: 39 :-0.000122061
---------------------
---------------------
msin: 40 :0.709539
dsin: 40 :0.709528
esin: 40 :1.0831e-05
---------------------
---------------------
msin: 41 :-0.726324
dsin: 41 :-0.726327
esin: 41 :3.01928e-06
---------------------
---------------------
msin: 42 :0.721873
dsin: 42 :0.721774
esin: 42 :9.85768e-05
---------------------
---------------------
msin: 43 :0.128535
dsin: 43 :0.128554
esin: 43 :-1.95125e-05
---------------------
---------------------
msin: 44 :-0.876126
dsin: 44 :-0.875956
esin: 44 :-0.00017037
---------------------
---------------------
msin: 45 :0.961365
dsin: 45 :0.960983
esin: 45 :0.000382119
---------------------
---------------------
msin: 46 :-0.734421
dsin: 46 :-0.734225
esin: 46 :-0.000195845
---------------------
---------------------
msin: 47 :-0.794548
dsin: 47 :-0.794536
esin: 47 :-1.1789e-05
---------------------
---------------------
msin: 48 :0.520956
dsin: 48 :0.520946
esin: 48 :9.56667e-06
---------------------
---------------------
msin: 49 :-0.802941
dsin: 49 :-0.802721
esin: 49 :-0.000220649
---------------------
---------------------
msin: 50 :0.974947
dsin: 50 :0.974947
esin: 50 :4.39906e-07
---------------------
---------------------
msin: 51 :0.438327
dsin: 51 :0.438188
esin: 51 :0.000139423
---------------------
---------------------
msin: 52 :0.0315795
dsin: 52 :0.0315563
esin: 52 :2.31417e-05
---------------------
---------------------
msin: 53 :0.4387
dsin: 53 :0.438696
esin: 53 :4.3264e-06
---------------------
---------------------
msin: 54 :0.53439
dsin: 54 :0.534351
esin: 54 :3.95051e-05
---------------------
---------------------
msin: 55 :-0.906202
dsin: 55 :-0.906023
esin: 55 :-0.000179272
---------------------
---------------------
msin: 56 :-0.413079
dsin: 56 :-0.41308
esin: 56 :4.71387e-07
---------------------
---------------------
msin: 57 :0.995999
dsin: 57 :0.995992
esin: 57 :6.74661e-06
---------------------
---------------------
msin: 58 :0.339239
dsin: 58 :0.339178
esin: 58 :6.00089e-05
---------------------
---------------------
msin: 59 :-0.427536
dsin: 59 :-0.427458
esin: 59 :-7.84904e-05
---------------------
---------------------
msin: 60 :-0.710931
dsin: 60 :-0.710923
esin: 60 :-8.04546e-06
---------------------
---------------------
msin: 61 :0.792618
dsin: 61 :0.792618
esin: 61 :4.34011e-07
---------------------
---------------------
msin: 62 :0.741526
dsin: 62 :0.741436
esin: 62 :8.93406e-05
---------------------
---------------------
msin: 63 :-0.89041
dsin: 63 :-0.890171
esin: 63 :-0.000239085
---------------------




Sine = 



logQ = 1200

ctres.logq = 840


END OF THE PROGRAMM
root@vultr:/home/sunly/Downloads/HomomorphicComparison-master/HomomorphicComparison/Default# 

```
