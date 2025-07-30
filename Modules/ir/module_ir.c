#include "module_ir.h"
#include "string.h"
#include "i2c.h"
module_ir_t *module_ir = NULL;

void set_adjust_mode()
{
    uint8_t mode = 0x01; // Adjust mode
    HAL_I2C_Mem_Write(&hi2c2, (0x12<<1), 0x01, I2C_MEMADD_SIZE_8BIT, &mode, 1, 10);
}

module_ir_t* ir_init()
{
    module_ir_t *ir_module = (module_ir_t *)malloc(sizeof(module_ir_t));
    memset(ir_module, 0, sizeof(ir_module));
    module_ir = ir_module;
    return ir_module;
}

static void read_IRdata()
{
    HAL_I2C_Mem_Read(&hi2c2, (0x12<<1), 0x30, I2C_MEMADD_SIZE_8BIT, module_ir->ir0, 1, 10);
}
void deal_IRdata()
{
    module_ir->ir0 = 0xff; // Initialize IR data
	read_IRdata();
    ir_done(module_ir); // Process the IR data
}

void ir_done(module_ir_t *m_ir)
{
    m_ir->ir_data[0] = (m_ir->ir0 >> 7)&0x01; // Store IR data in the buffer
    m_ir->ir_data[1] = (m_ir->ir0 >> 6)&0x01; // Store IR data in the buffer
	m_ir->ir_data[2] = (m_ir->ir0 >> 5)&0x01; // Store IR data in the buffer
	m_ir->ir_data[3] = (m_ir->ir0 >> 4)&0x01; // Store IR data in the buffer
	m_ir->ir_data[4] = (m_ir->ir0 >> 3)&0x01; // Store IR data in the buffer
	m_ir->ir_data[5] = (m_ir->ir0 >> 2)&0x01; // Store IR data in the buffer
	m_ir->ir_data[6] = (m_ir->ir0 >> 1)&0x01; // Store IR data in the buffer
	m_ir->ir_data[7] = (m_ir->ir0 >> 0)&0x01; // Store IR data in the buffer
}