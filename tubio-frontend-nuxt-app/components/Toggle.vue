<template>
<div>
    <div class="toggle-button">
        <IconToggleOff v-if="!isOn" />
        <IconToggleOn v-else />
    </div>
</div>
</template>

<script>
import IconToggleOn from "~/components/Icons/toggle-on.vue";
import IconToggleOff from "~/components/Icons/toggle-off.vue";

export default {
    props: {
        isOn: {type: Boolean, default: false}
    },
    components: {
      IconToggleOn,
      IconToggleOff,
  }
}
</script>

<style lang="scss" scoped>
.toggle-button {
  width: 50px;
  overflow: visible;
  stroke-width: 1px;
  fill: theme("colors.text-gray-1");
  stroke: theme("colors.text-gray-1");
  cursor: pointer;
  transition: fill 0.1s, stroke 0.1s;

  &:hover {
    fill: theme("colors.text-gray-2");
    stroke: theme("colors.text-gray-2");
  }
}
</style>
