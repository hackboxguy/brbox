This is a gpio control service supporting different h/w including raspberry pi 1 and 2 models.

use the following command on raspi-relay-power-strip to control those 4 relay's
ON : ./bin/gpioclt --gpio=17,1 --gpio=22,1 --gpio=23,1 --gpio=24,1
OFF: ./bin/gpioclt --gpio=17,0 --gpio=22,0 --gpio=23,0 --gpio=24,0
