<template>
    <div class="spacer"
        :style="'--height: ' + height + '; --m_height: ' + mobile_height + ';'" />
</template>

<script>
export default {
    props: {
        height: {type: String, default: "0"},
        m_height: {type: String, default: "0"},
    },

    computed: {
        mobile_height() {
            return (this.m_height === "0") ? this.height : this.m_height;
        },
    },

}
</script>

<style lang="scss" scoped>

.spacer {
  width: 1px;
  height: var(--height);

  @media (max-width: theme('screens.sm')) {
    height: var(--m_height);
  }
}

</style>
