#ifndef FLEXRAY_INJECTOR_RULES_H
#define FLEXRAY_INJECTOR_RULES_H

#include <stdint.h>

#define INJECT_DIRECTION_TO_VEHICLE 1
#define INJECT_DIRECTION_TO_ECU 0
typedef struct {
	uint16_t trigger_id;    // when this id arrives...
	uint16_t target_id;  // ...inject using cached template of this id (if available)
	uint8_t cycle_mask;
	uint8_t cycle_base;
	uint8_t e2e_offset;
	uint8_t e2e_len;
	uint8_t e2e_init_value;
	uint8_t replace_offset;
	uint8_t replace_len;
	uint8_t direction;
} trigger_rule_t;

static const trigger_rule_t INJECT_TRIGGERS[] = {
	// BMW i3 mimic rules.
	// For the first pass we keep the dynm-style injector engine but move away
	// from the old SP2018-only 0x47 -> 0x48 rule.
	//
	// These rules intentionally patch only the command-local slice while keeping
	// the rest of the cached OEM frame intact.
	//
	// cycle_mask = 0 / cycle_base = 0 means "accept any cycle" for now.
	// This keeps the firmware usable before the host starts supplying a tighter
	// phase-aware selection policy.
	{
		// Longitudinal brake-blend frame
		.trigger_id = 54,
		.target_id = 54,
		.cycle_mask = 0x00,
		.cycle_base = 0x00,
		.e2e_offset = 0,
		.e2e_len = 15,
		.e2e_init_value = 0xd6,
		.replace_offset = 3,
		.replace_len = 4,
		.direction = INJECT_DIRECTION_TO_ECU,
	},
	{
		// Longitudinal powertrain/coast frame
		.trigger_id = 59,
		.target_id = 59,
		.cycle_mask = 0x00,
		.cycle_base = 0x00,
		.e2e_offset = 0,
		.e2e_len = 15,
		.e2e_init_value = 0xd6,
		.replace_offset = 3,
		.replace_len = 4,
		.direction = INJECT_DIRECTION_TO_ECU,
	},
	{
		// Lateral outer envelope
		.trigger_id = 72,
		.target_id = 72,
		.cycle_mask = 0x00,
		.cycle_base = 0x00,
		.e2e_offset = 0,
		.e2e_len = 15,
		.e2e_init_value = 0xd6,
		.replace_offset = 0,
		.replace_len = 9,
		.direction = INJECT_DIRECTION_TO_ECU,
	},
	{
		// Lateral payload frame
		.trigger_id = 96,
		.target_id = 96,
		.cycle_mask = 0x00,
		.cycle_base = 0x00,
		.e2e_offset = 0,
		.e2e_len = 7,
		.e2e_init_value = 0xd6,
		.replace_offset = 0,
		.replace_len = 9,
		.direction = INJECT_DIRECTION_TO_ECU,
	},
};

#define NUM_TRIGGER_RULES (sizeof(INJECT_TRIGGERS)/sizeof(INJECT_TRIGGERS[0]))

#endif // FLEXRAY_INJECTOR_RULES_H

