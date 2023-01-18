Do delay and sum beamforming

./ds to list devices

Usage: ./ds [options] input_device[index] output_device[index]

`./ds --frames=4800 --channels=2 --margin=20 --sample_rate=48000 --display_levels=1 0 0`

Margin (Max(TDOA) = mic distance mm / (343000 / sample_rate) 2mic set to 8
Set display_levels=0 for silent running
--frames=4800 seems to work well the more samples fed to tdoa the more accurate but also latency increases 

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
60mm mic spacing @ 48Khz = 60 / (343000/48000) = 8.39 so a int margin of 8
16Khz only gives 2.798 so of no use

`/tmp/ds-out` contains current TDOA so poll to set LEDs
To monitor `watch -n 0.1 cat /tmp/ds-out`

To fix the beamformer write a file to `/tmp/ds-in`
`echo 1 > /tmp/ds-in` sets beam to a delay of 1
Delete file to clear and use TDOA

As an example with current setup
```
stuart@raspberrypi:~/2ch_delay_sum $ aplay -l
**** List of PLAYBACK Hardware Devices ****
card 0: Headphones [bcm2835 Headphones], device 0: bcm2835 Headphones [bcm2835 Headphones]
  Subdevices: 8/8
  Subdevice #0: subdevice #0
  Subdevice #1: subdevice #1
  Subdevice #2: subdevice #2
  Subdevice #3: subdevice #3
  Subdevice #4: subdevice #4
  Subdevice #5: subdevice #5
  Subdevice #6: subdevice #6
  Subdevice #7: subdevice #7
card 1: vc4hdmi [vc4-hdmi], device 0: MAI PCM i2s-hifi-0 [MAI PCM i2s-hifi-0]
  Subdevices: 1/1
  Subdevice #0: subdevice #0
card 2: seeed2micvoicec [seeed-2mic-voicecard], device 0: bcm2835-i2s-wm8960-hifi wm8960-hifi-0 [bcm2835-i2s-wm8960-hifi wm8960-hifi-0]
  Subdevices: 1/1
  Subdevice #0: subdevice #0
card 3: Loopback [Loopback], device 0: Loopback PCM [Loopback PCM]
  Subdevices: 1/1
  Subdevice #0: subdevice #0
card 3: Loopback [Loopback], device 1: Loopback PCM [Loopback PCM]
  Subdevices: 1/1
  Subdevice #0: subdevice #0
```
```
./ds
Devices:
0: (output) bcm2835 Headphones: - (hw:0,0) (ALSA)
1: () seeed-2mic-voicecard: bcm2835-i2s-wm8960-hifi wm8960-hifi-0 (hw:2,0) (ALSA)
2: () Loopback: PCM (hw:3,0) (ALSA)
3: () Loopback: PCM (hw:3,1) (ALSA)
4: (output) sysdefault (ALSA)
5: (output) lavrate (ALSA)
6: (output) samplerate (ALSA)
7: (output) speexrate (ALSA)
8: (output) upmix (ALSA)
9: (output) vdownmix (ALSA)
10: (output) playback (ALSA)
11: (input) capture (ALSA)
12: (output) dmixed (ALSA)
13: (input) array (ALSA)
14: (output) dmix (ALSA)
15: () default (ALSA) --default--
```
Use `./ds --frames=4800 --channels=2 --margin=8 --sample_rate=48000 --display_levels=0 1 2`

`arecord -Dplughw:3,1 -fS16_LE -r16000 -c1 beam.wav`

`make clean` to clean previous `make` to compile on your machine.

To make Pi audio a bit cleaner
`echo 'blacklist snd_bcm2835' | sudo tee /etc/modprobe.d/alsa-blacklist.conf`
Reboot
Also to auto load snd-aloop so,ething like
`echo 'options snd-aloop enable=1,1 index=0,1' | sudo tee /etc/modprobe.d/virtual-sound.conf`
really you just need one but the above is an example `options snd-aloop enable=1 index=0`
Disabling hdmi audio makes it even cleaner

