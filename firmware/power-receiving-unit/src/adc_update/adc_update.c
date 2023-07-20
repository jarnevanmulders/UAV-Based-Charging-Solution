#include "adc_update.h"


int16_t buf;

// Start advertisements
void adc_read_start(void){

    int err = 0;

    // *** SETUP ADC *** //
	struct adc_sequence sequence = {
		.buffer = &buf,
		/* buffer size in bytes, not number of samples */
		.buffer_size = sizeof(buf),
		.calibrate = true,
	};

    // /* Configure channels individually prior to sampling. */
	for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
		if (!device_is_ready(adc_channels[i].dev)) {
			printk("ADC controller device %s not ready\n", adc_channels[i].dev->name);
			return 0;
		}

		err = adc_channel_setup_dt(&adc_channels[i]);
		if (err < 0) {
			printk("Could not setup channel #%d (%d)\n", i, err);
			return 0;
		}
	}



	while (1)
	{
		// Check every 100 ms
		k_msleep(100);

        // Update ADC readings
		update_adc_readings(&sequence);


	}
}


// Update ADC voltages
void update_adc_readings(struct adc_sequence *sequence){
    int err = 0;
	for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {

		// Init ADC channel
		(void)adc_sequence_init_dt(&adc_channels[i], sequence);

		// Read ADC channel
		err = adc_read(adc_channels[i].dev, sequence);
		if (err < 0) {
			printk("Could not read (%d)\n", err);
			continue;
		}

		// Convert to mv
		int32_t val_mv;
		val_mv = (int32_t)buf;
		err = adc_raw_to_millivolts_dt(&adc_channels[i], &val_mv);

		// Convert ??? voltage divider ToDo

		// Store in global variable via pointer array
		*adc_results_m [i] = val_mv;
	}
}


