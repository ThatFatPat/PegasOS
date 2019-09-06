#pragma once

// Control registers

#define CR0_PE (1 << 0)
#define CR0_WP (1 << 16)
#define CR0_PG (1 << 31)

#define CR4_PAE (1 << 5)

// MSRs

#define MSR_EFER 0xC0000080
#define MSR_EFER_LME (1 << 8)
