<template>
  <div>

    <div class="hidden md:block">
      <Spacer height="0" m_height="1em" />
      <Logo class="logo mt-4" />
      <Spacer height="2em" m_height="2em" />
    </div>

    <div class="flex flex-row flex-wrap md:flex-no-wrap  input-flex justify-between md:justify-center">
      <input class="flex-grow md:mr-4 mb-2 md:mb-0 w-full"
        type="url"
        name="video_url"
        id="video_url"
        ref="video_url"
        placeholder="video-url"
        v-on:keydown="keyMonitor"
      />

      <div class="w-full md:hidden" />

      <div class="flex-shrink button-submit flex-grow-0">
        <select name="mode" id="mode" ref="mode" class="dropdown">
          <option value="video">Video</option>
          <option value="audio">Audio</option>
        </select>
      </div>

      <div class="button flex-shrink button-submit flex-grow-0 ml-3" v-on:click="downloadButtonClicked">
        <IconArrowRightSquare class="icon-button" />
      </div>

    </div>

    <Spacer height="2em" m_height="2em" />
    <DownloadBox />

    <PleaseWaitBlocker v-if="isWaitingForResponse" />

  </div>
</template>

<script>
import Logo from "~/components/Logo";
import Spacer from "~/components/Spacer";
import DownloadBox from "~/components/DownloadBox";
import PleaseWaitBlocker from "~/components/PleaseWaitBlocker";
import IconArrowRightSquare from "~/components/Icons/arrow-right-square";
import axios from "axios";

export default {
  components: {
    Logo,
    Spacer,
    DownloadBox,
    PleaseWaitBlocker,
    IconArrowRightSquare,
  },

  data: function() {
    return {
      isWaitingForResponse: {type: Boolean, default: false}
    };
  },

  methods: {
    downloadButtonClicked: function() {

      const that = this;
      if (this.$refs.video_url.value.match(/(https?:\/\/)?[a-zA-Z0-9-_.]+\.[a-zA-Z-_.]+/)) {
        const url = this.$refs.video_url.value;
        this.$refs.video_url.value = "";
        this.isWaitingForResponse = true;
        axios.post("/api", {
          request: "queue_download",
          video_url: url,
          mode: this.$refs.mode.options[this.$refs.mode.selectedIndex].value
        }).then(function(response){
          if (response.data.status === "OK") {
            that.isWaitingForResponse = false;
            that.$store.dispatch("dlcache/update", that);
          }
        });
      }
      return;
    },

    keyMonitor: function(event) {
      if (event.key == "Enter") {
        this.downloadButtonClicked();
      }
      return;
    }
  },

  mounted() {
    this.isWaitingForResponse = false;
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

.dropdown {
  min-width: 100px;
  height: 100%;
  border-radius: 5px;
  cursor: pointer;
  font-family: ZillaSlab, serif;
  font-size: 16pt;
}

</style>
