<template>
    <div class="click-blocker flex justify-center md:items-center">
        <div class="box">

            <div class="w-full h-full flex flex-col justify-center items-center">
              <h2 class="loading-text">Just a second...</h2>
              <div class="dots flex w-full justify-center items-center mt-24">
                  <div class="dot" />
                  <div class="dot" />
                  <div class="dot" />
              </div>
            </div>

        </div>        
    </div>
</template>

<script>
export default {

}
</script>

<style lang="scss" scoped>
.click-blocker {
  position: absolute;
  left: 0;
  right: 0;
  top: 0;
  bottom: 0;
}

.box {
  position: absolute;
  top: 0;
  left: 0;
  right: 0;
  height: 60%;
  overflow: hidden;

  @media (max-width: theme('screens.xl')) {
    position: absolute;
    top: 100px;
    bottom: 100px;
    left: 100px;
    right: 100px;
  }

  @media (max-width: theme('screens.md')) {
    position: absolute;
    top: 0;
    bottom: 0;
    left: 0;
    right: 0;
  }

  & h2 {
    text-align: center;
    font-size: 56pt;
    color: theme("colors.text-gray-1");

    @media (max-width: theme('screens.md')) {
      margin-top: 36px;
      max-height: unset;
    }
  }

  & .dots {
    @media (max-width: theme('screens.md')) {
      margin-top: 50px;
    }

    & .dot {
      width: 50px;
      height: 50px;
      background-color: theme("colors.gray-1");
      border-radius: 50px;
      animation-fill-mode: both;
      animation-timing-function: ease-in-out;

      &:nth-child(1) {
        animation: dot-hover 1.5s infinite;
        animation-delay: 0;
      }

      &:nth-child(2) {
        animation: dot-hover 1.5s infinite;
        animation-delay: 0.3s;
      }

      &:nth-child(3) {
        animation: dot-hover 1.5s infinite;
        animation-delay: 0.6s;
      }

      @media (max-width: theme('screens.md')) {
        width: 30px;
        height: 30px;

        &:not(:first-child) {
          margin-left: 15px;
        }

        &:not(:last-child) {
          margin-right: 15px;
        }
      }

      @media (min-width: theme('screens.md')) {
        font-size: 36pt; /* using font-size, to influence the em scale in the dot-hover animation */

        &:not(:first-child) {
          margin-left: 25px;
        }

        &:not(:last-child) {
          margin-right: 25px;
        }
      }
    }
  }

  & .loading-text {
    font-size: 36pt;

    @media (max-width: theme('screens.md')) {
      font-size: 24pt;
    }
  }
}

@keyframes dot-hover {
  0% { transform: translateY(0); }
  50% { transform: translateY(1em); }
  100% { transform: translateY(0); }
}
</style>