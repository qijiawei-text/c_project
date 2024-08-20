#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/resource.h>

#include "optical_voa.pb-c.h"
#include "optical_edfa.pb-c.h"
#include "optical_port_info.pb-c.h"
#include "optical_port_managed.pb-c.h"
#include "optical_port_unmanaged.pb-c.h"
#include "optical_top.pb-c.h"

#include "util_inc.h"
#include "dev_api.h"
#include "telemetry_data.h"



#define TELEMETRY_PRINT(en, pcStr, args ...)              \
    do {                                                  \
        if (en)                                           \
            printf(pcStr, ## args);                       \
    } while (0)




static void
__PrintOpticalStream(OpticalStream *pstOpticalStreamInfo)
{
    int i = 0;
    int j = 0;
    OpticalData *pstOpticalDataInfo = NULL;
    OpticalVoa *pstOpticalVoa = NULL;
    OpticalEdfa *pstOpticalEdfa = NULL;
    OpticalUnmanagedPort *apstUnmanagedPort[PMD_UNMANAGED_PORT_NUM_MAX];
    OpticalManagedPort *apstManagedPort[PMD_MANAGED_PORT_NUM_MAX];

    OpticalVoaInfo *apstOpticalVoaInfo[PMD_VOA_NUM_MAX];
    OpticalEdfaInfo *apstOpticalEdfaInfo[PMD_EDFA_NUM_MAX];

    OpticalPortOsc *apstUnmangedPortOscInfo[PMD_PORT_OSC_NUM_MAX];
    OpticalPortOch *apstUnmangedPortOchInInfo[PMD_PORT_OCH_IN_NUM_MAX];
    OpticalPortOch *apstUnmangedPortOchOutInfo[PMD_PORT_OCH_OUT_NUM_MAX];

    OpticalPortOsc *apstMangedPortOscInfo[PMD_PORT_OSC_NUM_MAX];
    OpticalPortOch *apstMangedPortOchInInfo[PMD_PORT_OCH_IN_NUM_MAX];
    OpticalPortOch *apstMangedPortOchOutInfo[PMD_PORT_OCH_OUT_NUM_MAX];

    if (NULL == pstOpticalStreamInfo) {
        LOG_WRITE(EN_LOG_ERROR, "__PMD_PrintOpticalStream input ptr is null!");
        return;
    }

    memset(apstUnmanagedPort, 0, sizeof(apstUnmanagedPort));
    memset(apstManagedPort, 0, sizeof(apstManagedPort));
    memset(apstOpticalVoaInfo, 0, sizeof(apstOpticalVoaInfo));
    memset(apstOpticalEdfaInfo, 0, sizeof(apstOpticalEdfaInfo));
    memset(apstUnmangedPortOscInfo, 0, sizeof(apstUnmangedPortOscInfo));
    memset(apstUnmangedPortOchInInfo, 0, sizeof(apstUnmangedPortOchInInfo));
    memset(apstUnmangedPortOchOutInfo, 0, sizeof(apstUnmangedPortOchOutInfo));
    memset(apstMangedPortOscInfo, 0, sizeof(apstMangedPortOscInfo));
    memset(apstMangedPortOchInInfo, 0, sizeof(apstMangedPortOchInInfo));
    memset(apstMangedPortOchOutInfo, 0, sizeof(apstMangedPortOchOutInfo));

    pstOpticalDataInfo = pstOpticalStreamInfo->optical_telemetry_data;
    pstOpticalVoa = pstOpticalDataInfo->voas;
    if (pstOpticalVoa != NULL) {
        for (i = 0; i < pstOpticalDataInfo->voas->n_voa_info; i++) {
            apstOpticalVoaInfo[i] = pstOpticalVoa->voa_info[i];
        }
    }

    pstOpticalEdfa = pstOpticalDataInfo->edfas;
    if (pstOpticalEdfa != NULL) {
        for (i = 0; i < pstOpticalEdfa->n_edfa_info; i++) {
            apstOpticalEdfaInfo[i] = pstOpticalEdfa->edfa_info[i];
        }
    }

    for (i = 0; i < pstOpticalDataInfo->n_unmanaged_port; i++) {
        apstUnmanagedPort[i] = pstOpticalDataInfo->unmanaged_port[i];
    }

    for (i = 0; i < pstOpticalDataInfo->n_managed_port; i++) {
        apstManagedPort[i] = pstOpticalDataInfo->managed_port[i];
    }

    TELEMETRY_PRINT(true, "system_id:%s\n", pstOpticalStreamInfo->system_id);
    TELEMETRY_PRINT(pstOpticalStreamInfo->timestamp, "timestamp:%ld\n", pstOpticalStreamInfo->timestamp);
    TELEMETRY_PRINT(pstOpticalStreamInfo->sequence_number, "sequence_number:%d\n", pstOpticalStreamInfo->sequence_number);
    TELEMETRY_PRINT(true, "version_major:%d\n", pstOpticalStreamInfo->version_major);
    TELEMETRY_PRINT(pstOpticalStreamInfo->version_minor, "version_minor:%d\n", pstOpticalStreamInfo->version_minor);

    for (i = 0; (pstOpticalVoa != NULL) && (i < pstOpticalVoa->n_voa_info); i++) {
        TELEMETRY_PRINT(apstOpticalVoaInfo[i]->voa_id, "voa%d voa_id:%d\n", i, apstOpticalVoaInfo[i]->voa_id);
        TELEMETRY_PRINT(apstOpticalVoaInfo[i]->target_atten, "voa%d target_atten:%f\n", i, apstOpticalVoaInfo[i]->target_atten);
        TELEMETRY_PRINT(apstOpticalVoaInfo[i]->pwr_in, "voa%d pwr_in:%f\n", i, apstOpticalVoaInfo[i]->pwr_in);
        TELEMETRY_PRINT(apstOpticalVoaInfo[i]->pwr_out, "voa%d pwr_out:%f\n", i, apstOpticalVoaInfo[i]->pwr_out);
        TELEMETRY_PRINT(apstOpticalVoaInfo[i]->measured_atten, "voa%d measured_atten:%f\n", i, apstOpticalVoaInfo[i]->measured_atten);
    }

    for (i = 0; (pstOpticalEdfa != NULL) && (i < pstOpticalEdfa->n_edfa_info); i++) {
        TELEMETRY_PRINT(apstOpticalEdfaInfo[i]->edfa_id, "edfa%d edfa_id:%d\n", i, apstOpticalEdfaInfo[i]->edfa_id);
        TELEMETRY_PRINT(apstOpticalEdfaInfo[i]->maint_state, "edfa%d maint_state:%d\n", i, apstOpticalEdfaInfo[i]->maint_state);
        TELEMETRY_PRINT(apstOpticalEdfaInfo[i]->oper_state, "edfa%d oper_state:%d\n", i, apstOpticalEdfaInfo[i]->oper_state);
        TELEMETRY_PRINT(apstOpticalEdfaInfo[i]->ctrl_mode, "edfa%d ctrl_mode:%d\n", i, apstOpticalEdfaInfo[i]->ctrl_mode);
        TELEMETRY_PRINT(apstOpticalEdfaInfo[i]->gain_switch_mode, "edfa%d gain_switch_mode:%d\n", i, apstOpticalEdfaInfo[i]->gain_switch_mode);
        TELEMETRY_PRINT(apstOpticalEdfaInfo[i]->target_gain, "edfa%d target_gain:%f\n", i, apstOpticalEdfaInfo[i]->target_gain);
        TELEMETRY_PRINT(apstOpticalEdfaInfo[i]->target_gain_tilt, "edfa%d target_gain_tilt:%f\n", i, apstOpticalEdfaInfo[i]->target_gain_tilt);
        TELEMETRY_PRINT(apstOpticalEdfaInfo[i]->pwr_in, "edfa%d pwr_in:%f\n", i, apstOpticalEdfaInfo[i]->pwr_in);
        TELEMETRY_PRINT(apstOpticalEdfaInfo[i]->pwr_out, "edfa%d pwr_out:%f\n", i, apstOpticalEdfaInfo[i]->pwr_out);
        TELEMETRY_PRINT(apstOpticalEdfaInfo[i]->measured_gain, "edfa%d measured_gain:%f\n", i, apstOpticalEdfaInfo[i]->measured_gain);
    }

    for (i = 0; i < pstOpticalDataInfo->n_unmanaged_port; i++) {
        TELEMETRY_PRINT(apstUnmanagedPort[i]->port_id, "unmanaged port%d port_id:%d\n", i, apstUnmanagedPort[i]->port_id);
        TELEMETRY_PRINT(apstUnmanagedPort[i]->port_data->total_pwr_in, "unmanaged port%d total_pwr_in:%f\n", i, apstUnmanagedPort[i]->port_data->total_pwr_in);
        TELEMETRY_PRINT(apstUnmanagedPort[i]->port_data->total_pwr_out, "unmanaged port%d total_pwr_out:%f\n", i, apstUnmanagedPort[i]->port_data->total_pwr_out);
        TELEMETRY_PRINT(apstUnmanagedPort[i]->port_data->oms_pwr_in, "unmanaged port%d oms_pwr_in:%f\n", i, apstUnmanagedPort[i]->port_data->oms_pwr_in);
        TELEMETRY_PRINT(apstUnmanagedPort[i]->port_data->oms_pwr_out, "unmanaged port%d oms_pwr_out:%f\n", i, apstUnmanagedPort[i]->port_data->oms_pwr_out);
        TELEMETRY_PRINT(apstUnmanagedPort[i]->port_data->oms_backreflection_pwr, "unmanaged port%d oms_backreflection_pwr:%f\n", i, apstUnmanagedPort[i]->port_data->oms_backreflection_pwr);
        TELEMETRY_PRINT(apstUnmanagedPort[i]->port_data->oms_return_loss, "unmanaged port%d oms_return_loss:%f\n", i, apstUnmanagedPort[i]->port_data->oms_return_loss);

        for (j = 0; j < apstUnmanagedPort[i]->port_data->n_osc_info; j++) {
            apstUnmangedPortOscInfo[j] = apstUnmanagedPort[i]->port_data->osc_info[j];
            TELEMETRY_PRINT(apstUnmangedPortOscInfo[j]->osc_pwr_in, "unmanaged port%d osc%d osc_pwr_in:%f\n", i, j, apstUnmangedPortOscInfo[j]->osc_pwr_in);
            TELEMETRY_PRINT(apstUnmangedPortOscInfo[j]->osc_pwr_out, "unmanaged port%d osc%d osc_pwr_out:%f\n", i, j, apstUnmangedPortOscInfo[j]->osc_pwr_out);
            TELEMETRY_PRINT(apstUnmangedPortOscInfo[j]->osc_wavelength, "unmanaged port%d osc%d osc_wavelength:%f\n", i, j, apstUnmangedPortOscInfo[j]->osc_wavelength);
        }

        for (j = 0; j < apstUnmanagedPort[i]->port_data->n_och_info_in; j++) {
            apstUnmangedPortOchInInfo[j] = apstUnmanagedPort[i]->port_data->och_info_in[j];
            TELEMETRY_PRINT(apstUnmangedPortOchInInfo[j]->och_prov_start_freq, "unmanaged port%d ochin%d och_prov_start_freq:%f\n", i, j, apstUnmangedPortOchInInfo[j]->och_prov_start_freq);
            TELEMETRY_PRINT(apstUnmangedPortOchInInfo[j]->och_measured_freq, "unmanaged port%d ochin%d och_measured_freq:%f\n", i, j, apstUnmangedPortOchInInfo[j]->och_measured_freq);
            TELEMETRY_PRINT(apstUnmangedPortOchInInfo[j]->och_measured_pwr, "unmanaged port%d ochin%d och_measured_pwr:%f\n", i, j, apstUnmangedPortOchInInfo[j]->och_measured_pwr);
        }

        for (j = 0; j < apstUnmanagedPort[i]->port_data->n_och_info_out; j++) {
            apstUnmangedPortOchOutInfo[j] = apstUnmanagedPort[i]->port_data->och_info_out[j];
            TELEMETRY_PRINT(apstUnmangedPortOchOutInfo[j]->och_prov_start_freq, "unmanaged port%d ochout%d och_prov_start_freq:%f\n", i, j, apstUnmangedPortOchOutInfo[j]->och_prov_start_freq);
            TELEMETRY_PRINT(apstUnmangedPortOchOutInfo[j]->och_measured_freq, "unmanaged port%d ochout%d och_measured_freq:%f\n", i, j, apstUnmangedPortOchOutInfo[j]->och_measured_freq);
            TELEMETRY_PRINT(apstUnmangedPortOchOutInfo[j]->och_measured_pwr, "unmanaged port%d ochout%d och_measured_pwr:%f\n", i, j, apstUnmangedPortOchOutInfo[j]->och_measured_pwr);
        }
    }

    for (i = 0; i < pstOpticalDataInfo->n_managed_port; i++) {
        TELEMETRY_PRINT(apstManagedPort[i]->port_id, "managed port%d port_id:%d\n", i, apstManagedPort[i]->port_id);
        TELEMETRY_PRINT(apstManagedPort[i]->maint_state, "managed port%d maint_state:%d\n", i, apstManagedPort[i]->maint_state);
        TELEMETRY_PRINT(apstManagedPort[i]->oper_state, "managed port%d oper_state:%d\n", i, apstManagedPort[i]->oper_state);
        TELEMETRY_PRINT(apstManagedPort[i]->port_data->total_pwr_in, "managed port%d total_pwr_in:%f\n", i, apstManagedPort[i]->port_data->total_pwr_in);
        TELEMETRY_PRINT(apstManagedPort[i]->port_data->total_pwr_out, "managed port%d total_pwr_out:%f\n", i, apstManagedPort[i]->port_data->total_pwr_out);
        TELEMETRY_PRINT(apstManagedPort[i]->port_data->oms_pwr_in, "managed port%d oms_pwr_in:%f\n", i, apstManagedPort[i]->port_data->oms_pwr_in);
        TELEMETRY_PRINT(apstManagedPort[i]->port_data->oms_pwr_out, "managed port%d oms_pwr_out:%f\n", i, apstManagedPort[i]->port_data->oms_pwr_out);
        TELEMETRY_PRINT(apstManagedPort[i]->port_data->oms_backreflection_pwr, "managed port%d oms_backreflection_pwr:%f\n", i, apstManagedPort[i]->port_data->oms_backreflection_pwr);
        TELEMETRY_PRINT(apstManagedPort[i]->port_data->oms_return_loss, "managed port%d oms_return_loss:%f\n", i, apstManagedPort[i]->port_data->oms_return_loss);

        for (j = 0; j < apstManagedPort[i]->port_data->n_osc_info; j++) {
            apstMangedPortOscInfo[j] = apstManagedPort[i]->port_data->osc_info[j];
            TELEMETRY_PRINT(apstMangedPortOscInfo[j]->osc_pwr_in, "managed port%d osc%d osc_pwr_in:%f\n", i, j, apstMangedPortOscInfo[j]->osc_pwr_in);
            TELEMETRY_PRINT(apstMangedPortOscInfo[j]->osc_pwr_out, "managed port%d osc%d osc_pwr_out:%f\n", i, j, apstMangedPortOscInfo[j]->osc_pwr_out);
            TELEMETRY_PRINT(apstMangedPortOscInfo[j]->osc_wavelength, "managed port%d osc%d osc_wavelength:%f\n", i, j, apstMangedPortOscInfo[j]->osc_wavelength);
        }

        for (j = 0; j < apstManagedPort[i]->port_data->n_och_info_in; j++) {
            apstMangedPortOchInInfo[j] = apstManagedPort[i]->port_data->och_info_in[j];
            TELEMETRY_PRINT(apstMangedPortOchInInfo[j]->och_prov_start_freq, "managed port%d ochin%d och_prov_start_freq:%f\n", i, j, apstMangedPortOchInInfo[j]->och_prov_start_freq);
            TELEMETRY_PRINT(apstMangedPortOchInInfo[j]->och_measured_freq, "managed port%d ochin%d och_measured_freq:%f\n", i, j, apstMangedPortOchInInfo[j]->och_measured_freq);
            TELEMETRY_PRINT(apstMangedPortOchInInfo[j]->och_measured_pwr, "managed port%d ochin%d och_measured_pwr:%f\n", i, j, apstMangedPortOchInInfo[j]->och_measured_pwr);
        }

        for (j = 0; j < apstManagedPort[i]->port_data->n_och_info_out; j++) {
            apstMangedPortOchOutInfo[j] = apstManagedPort[i]->port_data->och_info_out[j];
            TELEMETRY_PRINT(apstMangedPortOchOutInfo[j]->och_prov_start_freq, "managed port%d ochout%d och_prov_start_freq:%f\n", i, j, apstMangedPortOchOutInfo[j]->och_prov_start_freq);
            TELEMETRY_PRINT(apstMangedPortOchOutInfo[j]->och_measured_freq, "managed port%d ochout%d och_measured_freq:%f\n", i, j, apstMangedPortOchOutInfo[j]->och_measured_freq);
            TELEMETRY_PRINT(apstMangedPortOchOutInfo[j]->och_measured_pwr, "managed port%d ochout%d och_measured_pwr:%f\n", i, j, apstMangedPortOchOutInfo[j]->och_measured_pwr);
        }
    }
}

void
TELEMETRY_UnpackOpticalStream(const uint8_t *pucMsgBuf, size_t nMsgLen)
{
    OpticalStream *pstOpticalStreamInfo = NULL;

    if (NULL == pucMsgBuf) {
        printf("TELEMETRY_UnpackOpticalStream input ptr is null!");
        return;
    }

    pstOpticalStreamInfo = optical_stream__unpack(NULL, nMsgLen, pucMsgBuf);
    if (NULL == pstOpticalStreamInfo) {
        printf("Error optical_stream__unpack execute failed!");
        return;
    }

    __PrintOpticalStream(pstOpticalStreamInfo);

    /* free data */
    optical_stream__free_unpacked(pstOpticalStreamInfo, NULL);

    return;
}




