config-pin p9.15 gpio
config-pin p9.17 gpio
config-pin p9.21 pwm

config-pin p9.16 gpio
config-pin p9.18 gpio
config-pin p9.14 pwm

echo low > /sys/class/gpio/gpio48/direction
echo low > /sys/class/gpio/gpio5/direction

echo low > /sys/class/gpio/gpio4/direction
echo low > /sys/class/gpio/gpio51/direction

echo 1000000000 > /dev/bone/pwm/0/b/period
echo 500000000 > /dev/bone/pwm/0/b/duty_cycle

echo 1000000000 > /dev/bone/pwm/1/a/period
echo 500000000 > /dev/bone/pwm/1/a/duty_cycle

sleep 5

echo high > /sys/class/gpio/gpio48/direction
echo high > /sys/class/gpio/gpio51/direction

sleep 5

echo low > /sys/class/gpio/gpio48/direction
echo low > /sys/class/gpio/gpio51/direction

echo high > /sys/class/gpio/gpio5/direction
echo high > /sys/class/gpio/gpio4/direction

sleep 5

echo low > /sys/class/gpio/gpio5/direction
echo low > /sys/class/gpio/gpio4/direction

