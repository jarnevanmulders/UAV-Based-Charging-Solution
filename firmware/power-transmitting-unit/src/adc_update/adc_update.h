#ifndef ADC_UPDATE_H_
#define ADC_UPDATE_H_

#include <stdint.h>
#include <zephyr/kernel.h>

// ADC includes
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>


// *** ADC - Device three - check *** //
#if !DT_NODE_EXISTS(DT_PATH(zephyr_user)) || \
	!DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
#error "No suitable devicetree overlay specified"
#endif

#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
	ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

/* Data of ADC io-channels specified in devicetree. */
static const struct adc_dt_spec adc_channels[] = {
	DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels,
			     DT_SPEC_AND_COMMA)
};


void adc_read_start();

#endif /* ADC_UPDATE_H_ */


