To get cppssh and all dependant software:

```
repo init -u https://github.com/cdesjardins/cppsshManifest.git 
repo sync
cd build
[./]makebotan.py
[./]build.py --CDLogger --cppssh
```


cppssh uses the Botan library, latest version<br>
https://github.com/randombit/botan <br>
http://botan.randombit.net/ <br>

