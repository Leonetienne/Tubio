<template>
  <div>
    <div class="bgdrop"></div>
    <div class="bggatter__wrapper">
      <div class="bggatter" ref="bggatter" :style="cssVars"></div>
    </div>
    <div class="bgmask"></div>
  </div>
</template>

<script>
export default {
  data() {
    return {
      offsetX: { type: Number, default: 0 },
    };
  },
  computed: {
    cssVars() {
      return "transform: translateY(-" + this.offsetX / 2 + "px);";
    }
  },
  methods: {
    handleScroll: function(e) {
      this.offsetX = window.scrollY;
    }
  },
  beforeMount() {
    window.addEventListener("scroll", this.handleScroll);
  },
  destroyed() {
      window.removeEventListener("scroll", this.handleScroll);
    },
};
</script>

<style lang="scss" scoped>
.bgdrop {
  width: 100vw;
  height: 100vh;
  position: fixed;
  z-index: -10;
  background-color: theme("colors.purple-0");
}

.bggatter {
  width: 100%;
  height: 1000000px;
  position: absolute;
  z-index: -9;
  mask-image: url("/assets/images/tiles/grid.png");
  background-color: theme("colors.purple-1");

  &__wrapper {
    width: 100vw;
    height: 100vh;
    position: fixed;
    overflow: hidden;
    z-index: -9;
  }
}

.bgmask {
  width: 100vw;
  height: 100vh;
  position: fixed;
  z-index: -8;
  background-image: linear-gradient(#0000 50%, theme("colors.purple-0"));
}
</style>
