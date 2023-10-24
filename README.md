# nature-2
Well, so I rewrote the nature simulator in QT. 

## Ideas
Now the rendering takes place in a separate window, where there is grass, predators, herbivores, carnivorous plants (colored circles), fire. Simulation parameters can be set from the trava.h file. Later, I’ll probably rewrite the code for multithreading and add an optimized search algorithm (it’s already been developed, just needs to be integrated). In general, an interesting thing. I will develop the project if I continue to work in C++.

Ideas: genetic algorithms, improved reproduction mechanics, balancing algorithms. The most ambitious goal is to write a neuron for each unit, but this is difficult. If you want, you can help)

## acceleration and deceleration
By default it works in accelerated mode. to make it slower (update by seconds) comment out update() in draw.cpp, void draw::paintevent() line 380
```Cpp
    /*update();*/} else {
```
And uncomment void draw::timerEvent() update() line 151
```Cpp
void draw::timerEvent(QTimerEvent *)
{
        update();
}
```


