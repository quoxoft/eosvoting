Simple voting smart contract on EOS
-------
#### Prerequisites



##### Ceate voting account
````bash

````

##### Set volting contract to voting account
````bash
cleos set contract voting ./simple.volting -p voting
````

##### Register as a candidate
````bash
$ cleos push action voting regcandidate '["quoxoft", "EOS8UTtMXXCBG21GWQzq2pMmqpzoPrURWnAyqdWHBJBPxmpX6CT2M", "quoxoft.com"]' -p quoxoft
````

##### Get list candidate table 
````bash
$ cleos get table voting voting candidates
````

##### Vote for candidate
````bash
$ cleos push action voting vote '["helloeos", "quoxoft"]' -p helloeos
````
##### Get list voters
````bash
$ test get table voting voting voters
````
