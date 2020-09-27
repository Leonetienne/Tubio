<template>
  <div>

    <div class="hidden md:block">
      <Spacer height="5em" m_height="1em" />
      <Logo class="logo mt-4" />
      <Spacer height="2em" m_height="2em" />
    </div>

    <div class="flex flex-row input-flex justify-center">
      <input class="flex-grow mr-4" type="url" name="" id="" placeholder="video-url">

      <div class="button flex-shrink button-submit flex-grow-0">
        <IconArrowRightSquare class="icon-button" />
      </div>
    </div>

    <Spacer height="2em" m_height="2em" />
    <DownloadBox />

  </div>
</template>

<script>
import Logo from "~/components/Logo";
import Spacer from "~/components/Spacer";
import DownloadBox from "~/components/DownloadBox";
import IconArrowRightSquare from "~/components/Icons/arrow-right-square";

export default {
  components: {
    Logo,
    Spacer,
    DownloadBox,
    IconArrowRightSquare,
  }
};
</script>

<style lang="scss" scoped>
.logo {
  --logo-color: theme("colors.purple-3");

  width: 300px;
  fill: var(--logo-color);
  stroke: var(--logo-color);
  filter: drop-shadow(0 0 3px theme("colors.purple-3"));
  transition:
    fill 0.2s,
    stroke 0.2s,
    filter 0.2s;

  @media (max-width: theme('screens.md')) {
    width: 80px;
    align-self: flex-start;
  }

  @media (min-width: theme('screens.md')) {
    &:hover {
      --logo-color: theme("colors.purple-3-1");

      filter: drop-shadow(0 0 5px theme("colors.purple-3-1"));
    }
  }
}

.input-flex {
  width: 100%;
}

input {
  height: 50px;
  font-family: ZillaSlab, serif;
  font-weight: bold;
  font-size: 18pt;
  border-radius: 5px;
  padding: 0.1em 0.5em;
  box-shadow: 0 0 20px 5px #fff3;
  border: none;
  color: theme("colors.purple-0");
  transition: transform 0.2s;
  min-width: 0;

  &:focus {
    transform: scale(1.015);
  }
}

.icon {
  &-button {
    height: 2em;
    stroke: theme("colors.purple-0");
    overflow: hidden;
    transition:
      stroke 0.2s,
      fill 0.2s,
      color 0.2s;

    & svg {
      height: 100%;
    }
  }
}

.button {
  transition:
    transform 0.2s,
    background-color 0.2s,
    border 0.2s;

  &:hover {
    transform: scale(1.05);
    background-color: theme("colors.purple-1");
    color: #fff;

    & svg {
      fill: #fff;
      stroke: #fff;
    }
  }
}

</style>
