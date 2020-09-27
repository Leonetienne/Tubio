<template>
  <div class="header flex flex-row justify-between w-full p-5">

    <div class="left-icons flex flex-flex-row justify-start">
        <div class="headerbutton block md:hidden">
            <Logo class="headerlogo" />
        </div>
    </div>

    <div class="right-icons flex flex-row justify-end flex-grow">
        <div class="headerbutton button-settings">
            <IconGear class="headericon icon-gear" />
        </div>
    </div>

  </div>
</template>

<script>
import IconGear from "~/components/Icons/gear";
import Logo from "~/components/Logo";

export default {
  components: {
    IconGear,
    Logo,
  },
};
</script>

<style lang="scss" scoped>
.headericon {
  cursor: pointer;
  height: 50px;
  width: 50px;
  stroke: theme("colors.purple-3");
  fill: theme("colors.purple-3");
  -webkit-tap-highlight-color: #0000;
  transition:
    stroke 0.2s,
    fill 0.2s,
    color 0.2s,
    transform 0.2s ease-in-out;

  &:hover {
    transform: rotate(90deg);
    stroke: theme("colors.purple-3-1");
    fill: theme("colors.purple-3-1");
  }
}

.headerlogo {
  cursor: pointer;
  height: 60px;
  width: 60px;
  fill: theme("colors.purple-3");
  stroke: theme("colors.purple-3");
  -webkit-tap-highlight-color: #0000;
  transition:
    stroke 0.2s,
    fill 0.2s,
    color 0.2s;

  &:hover {
    stroke: theme("colors.purple-3-1");
  }

  & svg {
    fill: var(theme("colors.purple-3"));
    stroke: var(theme("colors.purple-3"));
    height: 100%;
  }
}

.icon {
  &-gear {
    & svg {
      height: 100%;
    }
  }
}
</style>