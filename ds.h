/* Created on 2016-08-15
 * Author: Zhang Binbin
 * About: Delay and Sum implemention in c style
 */

#ifndef DS_H_
#define DS_H_
#include <cstdio>
#include <assert.h>

// Delay & Sum
// @params data : in format channel0, channel1

void DelayAndSum(const float *data, int num_channel, int num_sample,
                 int *tdoa, float *out, int margin) {
	for (int i = margin; i < (num_sample + margin); i++) {
        int count = 0;
        float sum = 0.0;
        for (int j = 0; j < num_channel; j++) {
            if (i + tdoa[j] >= 0 && i + tdoa[j] < (margin * 2) + num_sample ) {
				sum += data[j * (num_sample + margin * 2) + i + tdoa[j]];
                count++;
			//if (i < 32 || i > num_sample - 16) {
			//	printf("index=%d tdoa=%d ", j * (num_sample + margin * 2) + i + tdoa[j], tdoa[j]);
			//}
			//} else {
				//printf("Error=%d ", i + tdoa[j] >= 0 && i + tdoa[j] < (margin * 2) + num_sample);
			  }

        }
        assert(count > 0);
		out[i - margin] = sum / count;
		//if (i < 32 || i > num_sample - 16) {
		//	printf("index=%d ", i - margin);
		//}
    }
}

#endif
