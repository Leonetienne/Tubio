<template>
  <div>
    <Background />
    <Nuxt />
  </div>
</template>

<script>
import Background from "~/components/Layout/Background";

export default {
  components: {
    Background,
  }
};
</script>

<style lang="css" scoped>
  .dontcry {
    color: #fff;
  }
</style>
