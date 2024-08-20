#include "Phy.h"

#define MV1512_PHY_ADDR     0x00


bool
CPhy::Init()
{
    uint8_t ucDataW = 0;
    uint16_t usDataBuf = 0;
    LOG_WRITE(EN_LOG_NOTICE, "Init +++");

    if (nullptr == m_stpLogicDrv) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicDrv is nullptr.");
        return false;
    }

#if 1
    /* OSC 88e1512  */
    /* select phy1 */
    ucDataW = 0;
    m_stpLogicDrv->Write(0x202C, &ucDataW, 1);

    if (!m_stpMdio->Write(0, 0x16, 0x00)) {
        LOG_WRITE(EN_LOG_ERROR, "Mdio Write 8 0x16 0x00 error.");
        return false;
    }
    LOG_WRITE(EN_LOG_NOTICE, "Mdio Write 0 0x16 0x00.");
    LOG_WRITE(EN_LOG_NOTICE, "set page to usDataBuf:0x%x", usDataBuf);

    /* page 0 ,register 01 02 03 and 04 */

    m_stpMdio->Read(0, 0x01, &usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "read 0 1,usDataBuf:0x%x", usDataBuf);
    m_stpMdio->Read(0, 0x02, &usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "read 0 2,usDataBuf:0x%x", usDataBuf);
    m_stpMdio->Read(0, 0x03, &usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "read 0 3,usDataBuf:0x%x", usDataBuf);
    m_stpMdio->Read(0, 0x04, &usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "read 0 4,usDataBuf:0x%x", usDataBuf);


    /* page 18 ,register 20 */
    m_stpMdio->Write(0, 0x16, 18);
    LOG_WRITE(EN_LOG_NOTICE, "set page to 18");
    m_stpMdio->Read(0, 20, &usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "read p18 20,usDataBuf:0x%x", usDataBuf);

    usDataBuf &= 0xfff8;
    usDataBuf |= 0x0001;
    m_stpMdio->Write(0, 20, usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "register p18 20 write value:0x%x", usDataBuf);


    m_stpMdio->Read(0, 20, &usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "read p18 20,usDataBuf:0x%x", usDataBuf);

    sleep(3);
    m_stpMdio->Read(0, 20, &usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "read p18 20,usDataBuf:0x%x", usDataBuf);

    usDataBuf &= 0x7fff;
    usDataBuf |= 0x8000;
    m_stpMdio->Write(0, 20, usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "register 0x20 write value:0x%x,reset phy", usDataBuf);

    sleep(3);
    m_stpMdio->Read(0, 20, &usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "read 0 20,usDataBuf:0x%x", usDataBuf);


    /* page 3 ,register 16 */
    m_stpMdio->Write(0, 0x16, 3);
    LOG_WRITE(EN_LOG_NOTICE, "set page to 3");
    m_stpMdio->Read(0, 16, &usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "read p3 16,usDataBuf:0x%x", usDataBuf);

    usDataBuf &= 0xfff0;
    usDataBuf |= 0x0001;
    m_stpMdio->Write(0, 16, usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "register p3 16 write value:0x%x", usDataBuf);

#endif
#if 1
    /* OSC 88e1512	*/
    /* select phy2 */
    ucDataW = 0x10;
    m_stpLogicDrv->Write(0x202C, &ucDataW, 1);

    if (!m_stpMdio->Write(0, 0x16, 0x00)) {
        LOG_WRITE(EN_LOG_ERROR, "Mdio Write 8 0x16 0x00 error.");
        return false;
    }
    LOG_WRITE(EN_LOG_NOTICE, "Mdio Write 0 0x16 0x00.");
    LOG_WRITE(EN_LOG_NOTICE, "set page to usDataBuf:0x%x", usDataBuf);

    /* page 0 ,register 01 02 03 and 04 */

    m_stpMdio->Read(0, 0x01, &usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "read 0 1,usDataBuf:0x%x", usDataBuf);
    m_stpMdio->Read(0, 0x02, &usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "read 0 2,usDataBuf:0x%x", usDataBuf);
    m_stpMdio->Read(0, 0x03, &usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "read 0 3,usDataBuf:0x%x", usDataBuf);
    m_stpMdio->Read(0, 0x04, &usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "read 0 4,usDataBuf:0x%x", usDataBuf);


    /* page 18 ,register 20 */
    m_stpMdio->Write(0, 0x16, 18);
    LOG_WRITE(EN_LOG_NOTICE, "set page to 18");
    m_stpMdio->Read(0, 20, &usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "read p18 20,usDataBuf:0x%x", usDataBuf);

    usDataBuf &= 0xfff8;
    usDataBuf |= 0x0001;
    m_stpMdio->Write(0, 20, usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "register p18 20 write value:0x%x", usDataBuf);


    m_stpMdio->Read(0, 20, &usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "read p18 20,usDataBuf:0x%x", usDataBuf);

    sleep(3);
    m_stpMdio->Read(0, 20, &usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "read p18 20,usDataBuf:0x%x", usDataBuf);

    usDataBuf &= 0x7fff;
    usDataBuf |= 0x8000;
    m_stpMdio->Write(0, 20, usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "register 0x20 write value:0x%x,reset phy", usDataBuf);

    sleep(3);
    m_stpMdio->Read(0, 20, &usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "read 0 20,usDataBuf:0x%x", usDataBuf);

    /* page 3 ,register 16 */
    m_stpMdio->Write(0, 0x16, 3);
    LOG_WRITE(EN_LOG_NOTICE, "set page to 3");
    m_stpMdio->Read(0, 16, &usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "read p3 16,usDataBuf:0x%x", usDataBuf);

    usDataBuf &= 0xfff0;
    usDataBuf |= 0x0001;
    m_stpMdio->Write(0, 16, usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "register p3 16 write value:0x%x", usDataBuf);

#endif
#if 1
    /* OSC 88e1512	*/
    /* select phy3 */
    ucDataW = 0x20;
    m_stpLogicDrv->Write(0x202C, &ucDataW, 1);

    if (!m_stpMdio->Write(0, 0x16, 0x00)) {
        LOG_WRITE(EN_LOG_ERROR, "Mdio Write 8 0x16 0x00 error.");
        return false;
    }
    LOG_WRITE(EN_LOG_NOTICE, "Mdio Write 0 0x16 0x00.");
    LOG_WRITE(EN_LOG_NOTICE, "set page to usDataBuf:0x%x", usDataBuf);

    /* page 0 ,register 01 02 03 and 04 */

    m_stpMdio->Read(0, 0x01, &usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "read 0 1,usDataBuf:0x%x", usDataBuf);
    m_stpMdio->Read(0, 0x02, &usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "read 0 2,usDataBuf:0x%x", usDataBuf);
    m_stpMdio->Read(0, 0x03, &usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "read 0 3,usDataBuf:0x%x", usDataBuf);
    m_stpMdio->Read(0, 0x04, &usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "read 0 4,usDataBuf:0x%x", usDataBuf);


    /* page 18 ,register 20 */
    m_stpMdio->Write(0, 0x16, 18);
    LOG_WRITE(EN_LOG_NOTICE, "set page to 18");
    m_stpMdio->Read(0, 20, &usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "read p18 20,usDataBuf:0x%x", usDataBuf);

    usDataBuf &= 0xfff8;
    usDataBuf |= 0x0001;
    m_stpMdio->Write(0, 20, usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "register p18 20 write value:0x%x", usDataBuf);


    m_stpMdio->Read(0, 20, &usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "read p18 20,usDataBuf:0x%x", usDataBuf);

    sleep(3);
    m_stpMdio->Read(0, 20, &usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "read p18 20,usDataBuf:0x%x", usDataBuf);

    usDataBuf &= 0x7fff;
    usDataBuf |= 0x8000;
    m_stpMdio->Write(0, 20, usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "register 0x20 write value:0x%x,reset phy", usDataBuf);

    sleep(3);
    m_stpMdio->Read(0, 20, &usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "read 0 20,usDataBuf:0x%x", usDataBuf);

    /* page 3 ,register 16 */
    m_stpMdio->Write(0, 0x16, 3);
    LOG_WRITE(EN_LOG_NOTICE, "set page to 3");
    m_stpMdio->Read(0, 16, &usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "read p3 16,usDataBuf:0x%x", usDataBuf);

    usDataBuf &= 0xfff0;
    usDataBuf |= 0x0001;
    m_stpMdio->Write(0, 16, usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "register p3 16 write value:0x%x", usDataBuf);

#endif



    LOG_WRITE(EN_LOG_NOTICE, "Init ---");

    return true;
}



