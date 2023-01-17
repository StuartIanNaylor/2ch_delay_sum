Do delay and sum beamforming
Usage: ds [options] input_device[index] output_device[index]

`ds --frames=4800 --channels=2 --margin=20 --sample_rate=48000 --display_levels=1 0 0`

Margin (Max(TDOA) = mic distance mm / (343000 / sample_rate)
Set display_levels=0 for silent running

```
Options:
  --channels                  : mic input channels (int, default = 2)
  --display_levels            : display_levels (int, default = 1)
  --frames                    : frame buffer size (int, default = 4000)
  --margin                    : constraint for tdoa estimation (int, default = 16)
  --sample_rate               : sample rate (int, default = 48000)
```  

TDOA needs as high sample rate possible to get max resolution and frame set to approx 1/10 of sample_rate or above
Output to a asndloop loopback adapter to down sample to needed rate
https://sysplay.in/blog/linux/2019/06/playing-with-alsa-loopback-devices/
```
sudo modprobe snd-aloop
sudo apt-get install portaudio19-dev libportaudio2
```

Margin is the max TDOA which is dictated by mic distance (mm) divided by the factor of speed of sound (343 m/s) divided by sample rate
60mm mic spacing @ 48Khz = 60 / (343000/48000)
