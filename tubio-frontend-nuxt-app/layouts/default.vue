<template>
  <div>
    <Background />
    <Header />

    <div class="main-column mx-auto">
      <Nuxt class="w-full flex flex-col justify-start items-center"/>
    </div>

  </div>
</template>

<script>
import Background from "~/components/Layout/Background";
import Header from "~/components/Layout/Header";

export default {
  components: {
    Background,
    Header,
  }
};
</script>

<style lang="css" scoped>
.main-column {
  width: 80%;
  padding: 0 20% 50px 20%;
  background-color: #0005;
  box-shadow: 0 0 50px 50px #0005;
  min-height: 100vh;

  @media (max-width: theme('screens.md')) {
    width: 100%;
    padding: 0 20px;
    padding: 0 20px 50px 20px;
  }
}
</style>
