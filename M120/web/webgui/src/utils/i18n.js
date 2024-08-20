// translate router.meta.title, be used in breadcrumb sidebar 

export function generateTitle(title) {
  const hasKey = this.$te('route.' + title)
  const translatedTitle = this.$t('route.' + title) // $t :this method from vue-i18n, inject in @/lang/index.js
  if (hasKey) {
    return translatedTitle
  }
  return title
}
export function generateSkinColor(color) {
  const hasKey = this.$te('skin.' + color)
  const translatedTitle = this.$t('skin.' + color) // $t :this method from vue-i18n, inject in @/lang/index.js

  if (hasKey) {
    return translatedTitle
  }
  return color
}

export function generateInvalid(title) {
  const hasKey = this.$te('invalid.' + title)
  const translatedTitle = this.$t('invalid.' + title) // $t :this method from vue-i18n, inject in @/lang/index.js
  if (hasKey) {
    return translatedTitle
  }
  return title
}

export function generateForm(title) {
  const hasKey = this.$te('form.' + title)
  const translatedForm = this.$t('form.' + title) // $t :this method from vue-i18n, inject in @/lang/index.js
  if (hasKey) {
    return translatedForm
  }
  return title
}
