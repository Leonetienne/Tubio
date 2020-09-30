<template>
    <div class="frame mb-6 pt-2 pb-4">

        <div class="flex flex-col">

            <div class="flex items-end justify-between w-full md:w-auto">
                <div class="timestamp">
                    20.09.2020
                </div>

                <div class="button-remove">
                    <IconX />
                </div>
            </div>

            <div class="flex flex-col-reverse md:flex-row w-full mt-2">
            
                <div class="flex flex-col">
                    <a href="www.youtube.de" title="To the original source">
                        <div class="thumbnail flex-shrink-0">
                            <div class="thumbnail__vignette" />
                            <div class="thumbnail__duration">29:30</div>
                        </div>
                    </a>

                    <!-- Progressbar -->
                    <div v-if="false">
                        <div class="status--progressbar flex w-full mt-3">
                            <div class="status--progressbar__good items-stretch"></div>
                        </div>
                        <div class="status--progressbar__text">
                            69%
                        </div>
                    </div>

                    <!-- Ready -->
                    <a v-if="true" href="" title="download">
                        <div class="status--ready mt-3 button flex justify-center w-full">
                            <div>
                                <IconDownload />
                            </div>
                        </div>
                    </a>

                     <!-- Queued -->
                    <div v-if="false">
                        <div class="status--queued mt-3">
                            Queued
                        </div>
                    </div>

                    <!-- Failed -->
                    <div v-if="false">
                        <div class="status--failed mt-3">
                            Failed!
                        </div>
                    </div>

                </div>

                <div class="flex flex-col md:ml-4 overflow-x-hidden">
                    <h1 class="title pr">Versengold - Thekenmädchen (Offizielles Video) Versengold - Thekenmädchen (Offizielles Video) Versengold - Thekenmädchen (Offizielles Video)</h1>
                    
                    <div class="relative my-4">
                        <p class="description p-2">
                            Thekenmädchen - Die zweite Single aus dem neuen Album "NORDLICHT". Album-Release: 28.06.2019 
                            Hier das Album vorbestellen: https://Versengold.lnk.to/Nordlicht
                            Hier gehts zur Fanbox: https://Versengold.lnk.to/Nordlicht_Box

                            Hol dir den Song "Thekenmädchen" jetzt hier: https://Versengold.lnk.to/Thekenmaedchen 

                            Mehr Informationen unter: 
                            Webseite: http://www.versengold.com/
                            Facebook: https://www.facebook.com/Versengold
                        </p>
                        <div class="description__decobox description__decobox--left" />
                        <div class="description__decobox description__decobox--right" />
                    </div>
                </div>

            </div>
        </div>
    </div>
</template>

<script>
import IconDownload from "~/components/Icons/download.vue";
import IconX from "~/components/Icons/x.vue";

export default {
    components: {
        IconDownload,
        IconX
    }
}
</script>

<style lang="scss" scoped>
.frame {
  width: 100%;
  border-bottom: 2px solid theme("colors.gray-1");

  & .timestamp {
    font-family: ZillaSlab, serif;
    font-size: 12pt;
    color: theme("colors.text-gray-1");
  }

  & .thumbnail {
    background-image: url("https://i.ytimg.com/vi/wgfNsek8xkc/hqdefault.jpg?sqp=-oaymwEZCNACELwBSFXyq4qpAwsIARUAAIhCGAFwAQ==&rs=AOn4CLBrNQptU-Ni4VWrrsCm689gPnm2ww");
    background-size: cover;
    background-position: center;
    width: 150px;
    height: calc(150px * (9 / 16));
    position: relative;
    transition: transform 0.2s;
    cursor: pointer;
    scrollbar-width: none;

    @media (max-width: theme('screens.md')) {
      width: 100%;
      height: 130px;
    }

    &:hover {
      transform: scale(1.05);
    }

    &__vignette {
      position: absolute;
      top: 0;
      bottom: 0;
      right: 0;
      left: 0;
      box-shadow: 0 0 15px 2px #000;
    }

    &__duration {
      position: absolute;
      bottom: 3px;
      right: 3px;
      text-align: right;
      font-size: 12pt;
      color: theme("colors.text-gray-1");
    }
  }

  & .title {
    color: theme("colors.text-gray-1");
    font-size: 22pt;
    max-height: 1.05em;
    overflow-x: hidden;
    text-overflow: ellipsis;
    white-space: nowrap;
    transition: transform 20s;

    &:hover {
      overflow: visible;
      transform: translateX(-200%);
    }
  }

  & .description {
    color: theme("colors.text-gray-1");
    font-size: 14pt;
    height: 100px;
    overflow-x: hidden;
    overflow-y: scroll;
    position: relative;
    background-color: #0003;

    @media (max-width: theme('screens.md')) {
      overflow: scroll;
    }

    &__decobox {
      position: absolute;

      &--left {
        top: 0;
        left: 0;
        width: 40px;
        height: 40px;
        border-top: 2px solid theme("colors.gray-1");
        border-left: 2px solid theme("colors.gray-1");
      }

      &--right {
        bottom: 0;
        right: 0;
        width: 40px;
        height: 40px;
        border-bottom: 2px solid theme("colors.gray-1");
        border-right: 2px solid theme("colors.gray-1");
      }
    }
  }

  & .status {
    &--progressbar {
      background-color: #900;
      height: 5px;

      &__good {
        background-color: #0b0;
        width: 50%; // Download progress
      }

      &__text {
        font-size: 24pt;
        color: theme("colors.text-gray-1");
      }
    }

    &--ready {
      height: 45px;
      background-color: theme("colors.text-gray-1");
      transition:
        background-color 0.2s,
        border 0.2s;

      &:hover {
        background-color: theme("colors.purple-1");
        color: #fff;
      }

      & svg {
        height: 35px;
      }
    }

    &--queued {
      font-family: ZillaSlab, serif;
      font-size: 24pt;
      color: theme("colors.text-gray-1");
    }

    &--failed {
      font-family: ZillaSlab, serif;
      font-size: 24pt;
      color: theme("colors.text-error-1");
    }
  }

  & .button-remove {
    width: 35px;
    height: 35px;
    fill: theme("colors.gray-0");
    stroke: theme("colors.gray-0");
    cursor: pointer;
    transition:
      background-color 0.2s,
      border-radius 0.2s;

    &:hover {
      background-color: #d40;
      border-radius: 50%;
    }
  }

  &::-webkit-scrollbar,
  & *::-webkit-scrollbar {
    display: none;
  }
}
</style>
