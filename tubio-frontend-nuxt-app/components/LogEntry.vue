<template>
    <div>

        <p :class="mode">
            {{message}}
        </p>

    </div>
</template>

<script>
export default {
    props: {
        mode: {type: String, default: "normal"},
        message: {type: String}
    },
}
</script>

<style lang="scss" scoped>
.warn {
  color: theme("colors.text-gray-1");
  background-color: #fc04;
  font-size: 14pt;
}

.error {
  color: theme("colors.text-gray-1");
  background-color: #d404;
  font-size: 14pt;
}

.normal {
  color: theme("colors.text-gray-1");
  font-size: 14pt;
}
</style>
