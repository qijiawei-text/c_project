export class Enum {
  /**
   * 添加枚举字段
   * field: 枚举字段
   * label: 界面显示
   * value: 枚举值
   */
  add(field, label, value) {
    this[field] = { label, value }
    return this
  }

  /**
   * 根据枚举value获取其label
   */
  getLabelByValue(value) {
    // 字段不存在返回‘’
    if (value === undefined || value === null) {
      return ''
    }
    for (let i in this) {
      let e = this[i]
      if (e && e.value === value) {
        return e.label
      }
    }

    return ''
  }

  getLabel(label) {
    // 字段不存在返回‘’
    if (label === undefined || label === null) {
      return ''
    }
    for (let i in this) {
      let e = this[i]
      if (e && e.label === label) {
        return e.label
      }
    }
    return ''
  }


  getvaluebyLabel(label) {
    // 字段不存在返回‘’
    if (label === undefined || label === null) {
      // console.log("get label=", label);
      return ''
    }
    for (let i in this) {
      let e = this[i]
      console.log(i, e.label, label);
      if (e && e.label === label) {
        // console.log("================", e.value, e.label, label);
        return e.value
      }
    }

    return ''
  }
}

import DevOps from '../views/app/board/dev/DevOps.vue';
import DevOa from '../views/app/board/dev/DevOa.vue';
import DevOcm from '../views/app/board/dev/DevOcm.vue';
import DevVoa from '@/views/app/board/dev/DevVoa';
import DevOtdr from '@/views/app/board/dev/DevOtdr';
import DevTdcm from '../views/app/board/dev/DevTdcm.vue';
import Scc from '@/views/app/board/dev/Scc';
import PowerModule from '@/views/app/board/dev/PowerModule';
import Fan from '@/views/app/board/dev/Fan';
import DevCpld from '@/views/app/board/dev/DevCpld';
import CpMainBoard from '@/views/app/board/dev/CpMainBoard';
import CpOptical from '@/views/app/board/dev/CpOptical';

export default {
  uiCardTypeEnum: new Enum().add('CARD_PA', 'CARD_PA', null)
    .add('CARD_BA', 'CARD_BA', null)
    .add('OCM', 'OCM', null)
    .add('FAN', 'FAN', null)
    .add('POWER', 'POWER', null)
    .add('SCC', 'SCC', null)
    .add('CLASSIS', 'CLASSIS', null)
    .add('UNKOWN', 'UNKOWN', null),

  uiDevTypeEnum: new Enum().add('DEV_CHASSIS', 'DEV_CHASSIS', 'CardState')
    .add('DEV_SCC', 'DEV_SCC', 'CardState')
    .add('DEV_SLOT_INFO', 'DEV_SLOT_INFO', 'CardState')
    .add('DEV_OA', 'DEV_OA', DevOa)
    .add('DEV_VOA', 'DEV_VOA', 'CardState')
    .add('DEV_WSS', 'DEV_WSS', 'CardState')
    .add('DEV_OCM', 'DEV_OCM', DevOcm)
    .add('DEV_TDCM', 'DEV_TDCM', DevTdcm)
    .add('DEV_PAM4', 'DEV_PAM4', 'CardState')
    .add('DEV_OPS', 'DEV_OPS', DevOps)
    .add('DEV_DCE', 'DEV_DCE', 'CardState')
    .add('DEV_MCS', 'DEV_MCS', 'CardState')
    .add('DEV_TYPE_OSC', 'DEV_TYPE_OSC', 'CardState')
    .add('DEV_DGE', 'DEV_DGE', 'CardState')
    .add('DEV_OTDR', 'DEV_OTDR', 'CardState')
    .add('DEV_FPGA', 'DEV_FPGA', 'CardState')
    .add('DEV_CPLD', 'DEV_CPLD', 'CardState')
    .add('DEV_FAN', 'DEV_FAN', 'CardState')
    .add('DEV_LED', 'DEV_LED', 'CardState')
    .add('DEV_TEMP', 'DEV_TEMP', 'CardState')
    .add('DEV_POWER', 'DEV_POWER', 'CardState')
    .add('DEV_E2PROM', 'DEV_E2PROM', 'CardState'),


  EFuncType: new Enum().add('NONE', 'NONE', null)
    .add('OA_PA', 'OA_PA', null)
    .add('OA_BA', 'OA_BA', null)
    .add('OCM', 'OCM', null)
    .add('TDCM', 'TDCM', null)
    .add('LED', 'LED', null)
    .add('TEMP', 'TEMP', null)
    .add('MEM', 'MEM', null),

  CircuitPackComponentsEnum: new Enum().add('slot-chassis', 'slot-chassis', CpMainBoard)
    .add('slot-pm-1', 'slot-pm-1', PowerModule)
    .add('slot-pm-2', 'slot-pm-2', PowerModule)
    .add('slot-fan-1', 'slot-fan-1', Fan)
    .add('slot-fan-2', 'slot-fan-2', Fan)
    .add('slot-fan-3', 'slot-fan-3', Fan)
    .add('slot-fan-4', 'slot-fan-4', Fan)
    .add('slot-scc', 'slot-scc', Scc)
    .add('slot-optical', 'slot-optical', CpOptical)
    .add('cp-main-board', 'cp-main-board', CpMainBoard)
    .add('cpld-0', 'cpld-0', DevCpld)
    .add('cpld-1', 'cpld-1', DevCpld)
    .add('cp-power-1', 'cp-power-1', PowerModule)
    .add('cp-power-2', 'cp-power-2', PowerModule)
    .add('cp-fan-1', 'cp-fan-1', Fan)
    .add('cp-fan-2', 'cp-fan-2', Fan)
    .add('cp-fan-3', 'cp-fan-3', Fan)
    .add('cp-fan-4', 'cp-fan-4', Fan)
    .add('cp-optical', 'cp-optical', CpOptical)
    .add('edfa-pa-sw', 'edfa-pa-sw', DevOa)
    .add('edfa-pa-fpga', 'edfa-pa-fpga', DevOa)
    .add('edfa-ba-sw', 'edfa-ba-sw', DevOa)
    .add('edfa-ba-fpga', 'edfa-ba-fpga', DevOa)
    .add('ocm-sw', 'ocm-sw', DevOcm)
    .add('otdr-sw', 'otdr-sw', DevOtdr)
    .add('extend-sw', 'extend-sw', DevOa)

}