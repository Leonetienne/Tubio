<template>
  <div class="header flex flex-row justify-between w-full p-5">

    <div class="left-icons flex flex-flex-row justify-start">
      <nuxt-link exact to="/">
        <div class="headerbutton block md:hidden">
            <Logo class="headerlogo" />
        </div>
      </nuxt-link>
    </div>

    <div class="right-icons flex flex-row justify-end flex-grow">

        <a href="https://github.com/Leonetienne/Tubio" target="_blank" title="GitHub repository" class="mr-4">
          <div class="headerbutton flex justify-center items-center">
            <IconGithub class="headericon icon-github" />
          </div>
        </a>

        <nuxt-link exact to="/settings" title="Settings">
          <div class="headerbutton button-settings">
            <IconGear class="headericon icon-gear" />
          </div>
        </nuxt-link>

    </div>

  </div>
</template>

<script>
import IconGear from "~/components/Icons/gear";
import IconGithub from "~/components/Icons/github";
import Logo from "~/components/Logo";

export default {
  components: {
    IconGear,
    IconGithub,
    Logo,
  },
};
</script>

<style lang="scss" scoped>
.headerbutton {
  cursor: pointer;
  height: 50px;
  width: 50px;
}

.headericon {
  stroke: theme("colors.purple-3");
  fill: theme("colors.purple-3");
  transition:
    stroke 0.2s,
    fill 0.2s,
    color 0.2s,
    transform 0.2s ease-in-out;
}

.headerlogo {
  cursor: pointer;
  height: 60px;
  width: 60px;
  fill: theme("colors.purple-3");
  stroke: theme("colors.purple-3");
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

    &:hover {
      transform: rotate(90deg);
      stroke: theme("colors.purple-3-1");
      fill: theme("colors.purple-3-1");
    }
  }

  &-github {
    height: 40px;
    width: 40px;

    &:hover {
      transform: scale(1.075);
    }
  }
}
</style>