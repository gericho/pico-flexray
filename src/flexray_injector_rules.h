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
	// Trigger chain comes from observed raw route order:
	//   53 -> 54 -> 59 -> 72
	//   95 -> 96
	// This is closer to the dynm method than target==trigger and avoids
	// reinjecting on the same frame id that is being observed on the bus.
	//
	// These rules intentionally patch only the command-local slice while keeping
	// the rest of the cached OEM frame intact.
	//
	// Match dynm-style cycle gating: only inject on cycle_count mod 4 == 1.
	// This is intentionally stricter than the temporary bring-up rules and keeps
	// the injector aligned with a stable FlexRay subcycle.
	{
		// Longitudinal brake-blend frame
		.trigger_id = 53,
		.target_id = 54,
		.cycle_mask = 0x03,
		.cycle_base = 0x01,
		.e2e_offset = 0,
		.e2e_len = 15,
		.e2e_init_value = 0xd6,
		.replace_offset = 0,
		.replace_len = 9,
		.direction = INJECT_DIRECTION_TO_VEHICLE,
	},
	{
		// Longitudinal powertrain/coast frame
		.trigger_id = 54,
		.target_id = 59,
		.cycle_mask = 0x03,
		.cycle_base = 0x01,
		.e2e_offset = 0,
		.e2e_len = 15,
		.e2e_init_value = 0xd6,
		.replace_offset = 0,
		.replace_len = 9,
		.direction = INJECT_DIRECTION_TO_VEHICLE,
	},
	{
		// Lateral TJA higher-level gate A
		.trigger_id = 116,
		.target_id = 131,
		.cycle_mask = 0x03,
		.cycle_base = 0x01,
		.e2e_offset = 0,
		.e2e_len = 7,
		.e2e_init_value = 0xd6,
		.replace_offset = 0,
		.replace_len = 9,
		.direction = INJECT_DIRECTION_TO_VEHICLE,
	},
	{
		// Lateral TJA higher-level gate B.
		// Use self-target injection here like 72/96 so the cached 135 template
		// is patched on its own arrivals instead of depending on the 131->135
		// trigger/template timing pair.
		.trigger_id = 135,
		.target_id = 135,
		.cycle_mask = 0x00,
		.cycle_base = 0x00,
		.e2e_offset = 0,
		.e2e_len = 7,
		.e2e_init_value = 0xd6,
		.replace_offset = 0,
		.replace_len = 9,
		.direction = INJECT_DIRECTION_TO_VEHICLE,
	},
	{
		// Lateral bridge frame between envelope and helper context
		.trigger_id = 72,
		.target_id = 97,
		.cycle_mask = 0x03,
		.cycle_base = 0x01,
		.e2e_offset = 0,
		.e2e_len = 7,
		.e2e_init_value = 0xd6,
		.replace_offset = 0,
		.replace_len = 9,
		.direction = INJECT_DIRECTION_TO_VEHICLE,
	},
	{
		// Lateral TJA context helper A
		.trigger_id = 97,
		.target_id = 112,
		.cycle_mask = 0x03,
		.cycle_base = 0x01,
		.e2e_offset = 0,
		.e2e_len = 7,
		.e2e_init_value = 0xd6,
		.replace_offset = 0,
		.replace_len = 9,
		.direction = INJECT_DIRECTION_TO_VEHICLE,
	},
	{
		// Lateral TJA context helper B
		.trigger_id = 112,
		.target_id = 116,
		.cycle_mask = 0x03,
		.cycle_base = 0x01,
		.e2e_offset = 0,
		.e2e_len = 7,
		.e2e_init_value = 0xd6,
		.replace_offset = 0,
		.replace_len = 9,
		.direction = INJECT_DIRECTION_TO_VEHICLE,
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
		.direction = INJECT_DIRECTION_TO_VEHICLE,
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
		.direction = INJECT_DIRECTION_TO_VEHICLE,
	},
};

#define NUM_TRIGGER_RULES (sizeof(INJECT_TRIGGERS)/sizeof(INJECT_TRIGGERS[0]))

#endif // FLEXRAY_INJECTOR_RULES_H
