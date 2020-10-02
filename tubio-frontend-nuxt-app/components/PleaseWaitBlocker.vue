<template>
    <div class="click-blocker flex justify-center md:items-center">
        <div class="box">
            <h2 class="mt-2">Please wait...</h2>
        
            <div class="dots flex w-full justify-center items-center">
                <div class="dot" />
                <div class="dot" />
                <div class="dot" />
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
  margin-top: 160px;
  width: 1200px;
  height: 800px;
  max-height: 70vh;
  background-color: #586679;
  opacity: 0.95;
  border-radius: 5px;
  border: 2px solid theme("colors.gray-1");
  overflow: hidden;

  @media (max-width: theme('screens.md')) {
    margin-left: 15px;
    margin-right: 15px;
    height: 300px;
    margin-top: 80px;
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
    margin-top: 250px;

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
        width: 10px;
        height: 10px;

        &:not(:first-child) {
          margin-left: 5px;
        }

        &:not(:last-child) {
          margin-right: 5px;
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
}

@keyframes dot-hover {
  0% { transform: translateY(0); }
  50% { transform: translateY(1em); }
  100% { transform: translateY(0); }
}
</style>